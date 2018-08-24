#include <vector>
#include <deque>
#include <queue>
#include <iostream>

//!!!!What needs to be done!!!
//Need to merge used memory blocks
//Need to free memory blocks per write(512) 
//		call scheduler after one write

using namespace std;
extern "C" {
#include "VirtualMachine.h"
#include "Machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
//#include <vector>
//using namespace std;

volatile int msPerTick;
volatile unsigned int tickCount; //increment each callback is returned
//volatile int sleepcount;
volatile int numThreads; //includes all threads that has ever been
volatile int numPools;
volatile TVMThreadID currentTID;
volatile int num = 0;


#define VM_THREAD_PRIORITY_BOTTOM                  ((TVMThreadPriority)0x00)
const TVMMemoryPoolID VM_MEMORY_POOL_ID_SYSTEM = 0;

TVMMainEntry VMLoadModule(const char *module);
void VMUnloadModule(void);
TVMStatus VMFilePrint(int filedescriptor, const char *format, ...);
void scheduler(int wherefrom);
void freeSpace2(TVMMemoryPoolID memory, char* base);
char* findSpace(TVMMemoryPoolID memory, int length, int* index);


struct TCB{
	TVMThreadEntry entry;
	void* param;
	SMachineContext* context;
    TVMThreadID thid;
    volatile int sleepcount;
    volatile int ticksleft;
    void* stack;
    TVMMemorySize stacksize;
    TVMStatus state;
    TVMThreadPriority prio;
    int leftToProcess;
    int fd;
    int lengthRead;
    int lengthWriten;
    int offset;
    bool operator<(const TCB& rhs) const
    {
    	if (prio < rhs.prio)
    		return true;
    	else
    		return false;
    }
};


vector<TCB*> threads; //index 0 is main
deque<TCB*> sleepQ;
queue<TCB> readyQH;
queue<TCB> readyQN;
queue<TCB> readyQL;
queue<TCB> readyQB;

struct mutex{
	volatile bool locked;
	TVMThreadID owner;
	priority_queue<TCB> waitingQ;
	TVMMutexID mid;
};

//id: -1 ,fail
int getIndex(TVMThreadID thread)
{
	int id = -1;
	int i;
	for (i = 0; i < threads.size(); i++)
	{
		if(threads[i]->thid==thread)
			id = i;
	}
	return id;
}

vector<mutex*> mutices;

//For any mutex whose waitingQ is non-empty <-implicity actually
//	if unlocked, dequeue waitingQ, set locked and change owner, remove owner from sleepQ into readyQ
void Callback(void *calldata)
{
        if(threads.empty())
        {
		return;
	}
	TMachineSignalState sigstate;
	MachineSuspendSignals(&sigstate);
	TVMThreadID tempID;
	unsigned int j;

	tickCount++;
	for(j = 0; j < mutices.size(); j++)
	{
		//only interested in non-empty waitingQ of an unlocked mutex
		if(!mutices[j]->waitingQ.empty())
		{
			if(!mutices[j]->locked)
			{
				mutices[j]->locked = true;
				tempID = mutices[j]->waitingQ.top().thid;
				mutices[j]->owner = tempID;
				mutices[j]->waitingQ.pop();

				//look up thread to remove from sleepQ; push to readyQ
				unsigned int k;
				for (k = 0; k < sleepQ.size(); k++)
				{
					if(sleepQ[k]->thid == tempID)
						break;
				}

				if(k == sleepQ.size())
					cout << "\nsleepQ item not found in callBack";
				if(sleepQ[k]->state == VM_THREAD_STATE_DEAD)
					break;

				sleepQ[k]->state = VM_THREAD_STATE_READY;
				if(sleepQ[k]->prio == VM_THREAD_PRIORITY_HIGH)
					readyQH.push(*(sleepQ[k]));
				else if(sleepQ[k]->prio == VM_THREAD_PRIORITY_NORMAL)
					readyQN.push(*(sleepQ[k]));
				else if(sleepQ[k]->prio == VM_THREAD_PRIORITY_LOW)
					readyQL.push(*(sleepQ[k]));
				else
					readyQB.push(*(sleepQ[k]));
				sleepQ.erase(sleepQ.begin()+ k);
				MachineResumeSignals(&sigstate);
				scheduler(0);
			}
		}
	}
	if (threads[currentTID]->state != VM_THREAD_STATE_DEAD)
		threads[currentTID]->state = VM_THREAD_STATE_READY;
	if (currentTID != 1 || threads[currentTID]->state != VM_THREAD_STATE_DEAD)	
	{
		if(threads[currentTID]->prio == VM_THREAD_PRIORITY_HIGH)
			readyQH.push(*(threads[currentTID]));
		else if(threads[currentTID]->prio == VM_THREAD_PRIORITY_NORMAL)
			readyQN.push(*(threads[currentTID]));
		else if(threads[currentTID]->prio == VM_THREAD_PRIORITY_LOW)
			readyQL.push(*(threads[currentTID]));
		else
			readyQB.push(*(threads[currentTID]));
	}

	unsigned int i;
	for (i = 0; i < sleepQ.size(); i++)
	{
		sleepQ[i]->sleepcount--;
		if(sleepQ[i]->sleepcount == 0)
		{
			sleepQ[i]->state = VM_THREAD_STATE_READY;//change state to ready
			if(sleepQ[i]->prio == VM_THREAD_PRIORITY_HIGH)
				readyQH.push(*(sleepQ[i]));
			else if(sleepQ[i]->prio == VM_THREAD_PRIORITY_NORMAL)
				readyQN.push(*(sleepQ[i]));
			else if(sleepQ[i]->prio == VM_THREAD_PRIORITY_LOW)
				readyQL.push(*(sleepQ[i]));
			else
				readyQB.push(*(sleepQ[i]));
			sleepQ.erase(sleepQ.begin()+i);//pop from sleepQ
		}
	}
	MachineResumeSignals(&sigstate);
	//switch threads per tick	
	scheduler(0);
	return;	
}

//while with scheduler
void idleThreadEntry(void *param)
{
	MachineEnableSignals();
	while(1);
}

struct MemPool{
	int memId;
	int size; 
	int blockNum;// = floor(size/64);
	char* basePtr;
	//vector<char*> freeSpace; //points to beginning of blocks(64 in size)
	vector<int> isFree; //size == blockNum; -1 means free; positive depands on allocateNum
	int allocateNum;
};

vector<MemPool*> poolList;


int getIndex2(TVMMemoryPoolID memory)
{
	int id = -1;
	int i;
	for (i = 0; i < poolList.size(); i++)
	{
		if(poolList[i]->memId==memory)
			id = i;
	}
	return id;
}


//load module and run; 
TVMStatus VMStart(int tickms, TVMMemorySize heapsize, TVMMemorySize sharedsize, int argc, char *argv[])
{
	TVMMainEntry entry;
	tickCount = 0;
	msPerTick = tickms;	
	numThreads = 1;
	int i = 0;
	char* shareBase;

	entry = VMLoadModule(argv[0]);

	//main thread info
	TCB* mainThread = new TCB;
	SMachineContextRef mainContext = new SMachineContext;
	MachineContextSave(mainContext);
	mainThread->context = mainContext;
	mainThread->thid = 0;
	mainThread->fd = 0;
	mainThread->prio = VM_THREAD_PRIORITY_NORMAL;
	mainThread->state = VM_THREAD_STATE_RUNNING;
	mainThread->sleepcount = 0;
	threads.push_back(mainThread);
	currentTID = 0;

	TVMThreadID idleThreadID;
	TVMThreadEntry idleEntry;
	idleEntry = &idleThreadEntry;

	//sharedsize = 0x4000
	//heapsize = 0x1000000
	shareBase = (char*)MachineInitialize(sharedsize); //defalt ms/tick is 100
    MachineRequestAlarm(msPerTick*1000, Callback, NULL);//universal callback

    int init = -1;
	//create system pool and sharedmemory pool
    MemPool* sysPool = new MemPool;
    sysPool->basePtr = new char[heapsize];
    //cout << "basePtr adrress: " << static_cast<void*>(sysPool->basePtr) << endl;
    sysPool->memId = VM_MEMORY_POOL_ID_SYSTEM;
    sysPool->size = heapsize;
    sysPool->blockNum = heapsize/64; //set memory block numbers
    for(i = 0; i < sysPool->blockNum;i++)//initialize memory blocks free
    	sysPool->isFree.push_back(init);
    poolList.push_back(sysPool);

    MemPool* sharedMem = new MemPool;
    sharedMem->basePtr = shareBase;
    sharedMem->size = sharedsize;
    sharedMem->allocateNum = 0;
    sharedMem->blockNum = sharedsize/64; //set memory block numbers
    for(i = 0; i < sharedMem->blockNum;i++)//initialize memory blocks free
    	sharedMem->isFree.push_back(init);
    sharedMem->memId = 1;
    poolList.push_back(sharedMem);
    numPools = 2;

    //cout << "activating thread" << endl;
    VMThreadCreate(idleEntry, NULL, 0x100000, VM_THREAD_PRIORITY_BOTTOM, &idleThreadID);
	VMThreadActivate(idleThreadID);
	//cout << "finish activating\n";
	entry(argc, argv);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMMemoryPoolCreate(void *base, TVMMemorySize size, TVMMemoryPoolIDRef memory)
{
	int i;
	int init = -1;
	if(memory == NULL || size == 0 || base == NULL)
		return VM_STATUS_ERROR_INVALID_PARAMETER;

	MemPool* pool = new MemPool;
	pool->basePtr = (char*)base;
	pool->size = size;
	pool->blockNum = size/64;
	for(i = 0; i < pool->blockNum;i++)
    	pool->isFree.push_back(init);
	pool->memId = numPools;
	*memory = pool->memId;
	poolList.push_back(pool);
	numPools++;
	return VM_STATUS_SUCCESS;
}

TVMStatus VMMemoryPoolAllocate(TVMMemoryPoolID memory, TVMMemorySize size, void **pointer)
{
	int garbage;
	int index = getIndex2(memory);
	if(index == -1 || pointer == NULL || size == 0)
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	//find free + large enough block
	//change TVMMemoryPoolID's free list(mark used block)
		*pointer = findSpace(index, size, &garbage);
	if(!(*pointer))
		return VM_STATUS_ERROR_INSUFFICIENT_RESOURCES;
	return VM_STATUS_SUCCESS;
}

TVMStatus VMMemoryPoolDeallocate(TVMMemoryPoolID memory, void *pointer)
{
	int index = getIndex2(memory);
	if(index == -1 || pointer == NULL || ((char*)pointer - poolList[index]->basePtr) % 64 != 0)
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	if(pointer < poolList[index]->basePtr || pointer > poolList[index]->basePtr + poolList[index]->size)
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	freeSpace2(index, (char*)pointer);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMMemoryPoolDelete(TVMMemoryPoolID memory)
{
	unsigned int bytesleft;
	int index = getIndex2(memory);
	
	if(index >= poolList.size())
	{
		//cout << "memory location invalid\n";
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}

	VMMemoryPoolQuery(memory, &bytesleft);
	if(bytesleft < poolList[index]->size)
	{
		//cout << "memory havent been deallocated completely" << endl;
		return VM_STATUS_ERROR_INVALID_STATE;
	}

	poolList.erase(poolList.begin()+index);
	return VM_STATUS_SUCCESS;
}

//return free bytes
TVMStatus VMMemoryPoolQuery(TVMMemoryPoolID memory, TVMMemorySizeRef bytesleft)
{
	int i;
	int count = 0;

	int index = getIndex2(memory);
	if(index == -1 || bytesleft == NULL)
		return VM_STATUS_ERROR_INVALID_PARAMETER;

	int blockNum = poolList[index]->blockNum;
	MemPool* temp = poolList[index];

	for (i = 0; i < blockNum; i++)
	{
		if(temp->isFree[i] < 0)
			count++;
	}
	*bytesleft = count * 64;
	return VM_STATUS_SUCCESS;
}

TVMStatus VMTickMS(int *tickmsref)
{
	if(!tickmsref)
		return VM_STATUS_ERROR_INVALID_PARAMETER;
 	*tickmsref = msPerTick;
	return VM_STATUS_SUCCESS;
}

TVMStatus VMTickCount(TVMTickRef tickref)
{
	if(!tickref)
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	*tickref = tickCount;
	return VM_STATUS_SUCCESS;
}

TVMStatus VMThreadCreate(TVMThreadEntry entry, void *param, TVMMemorySize memsize, TVMThreadPriority prio, TVMThreadIDRef tid)
{
	if(entry == NULL || tid == NULL)
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	numThreads++;
	TCB* th = new TCB;
	void* tempPtr;
	//cout << "allocating space for thread" << endl;
	VMMemoryPoolAllocate(0, memsize, &tempPtr);
	//cout << "finshed" << endl << endl;

	th->stack = (char*)tempPtr;
	//th->stack = new char[memsize];
	th->context = new SMachineContext;
	th->stacksize = memsize;
	th->entry = entry;
	th->param = param;
	th->lengthRead = 0;
	th->lengthWriten = 0;
	th->ticksleft = 1;
	th->prio = prio;
	th->sleepcount = 0;
	*tid = numThreads-1;
	th->thid = *tid;
	th->state = VM_THREAD_STATE_DEAD;
	threads.push_back(th);

	return VM_STATUS_SUCCESS;
}

TVMStatus VMThreadDelete(TVMThreadID thread)
{
	int index;
	index = getIndex(thread);

	if(index < 0)
		return VM_STATUS_ERROR_INVALID_ID;
	if(threads[index]->state != VM_THREAD_STATE_DEAD)
		return VM_STATUS_ERROR_INVALID_STATE;

	threads.erase(threads.begin()+index);
	return VM_STATUS_SUCCESS;
}

void skeleton(void *param)
{
	MachineEnableSignals();
	TCB* currentTCB = (TCB*)param;
	currentTCB->entry(currentTCB->param);

	VMThreadTerminate(currentTCB->thid);
}

TVMStatus VMThreadActivate(TVMThreadID thread) //activates a dead thread, call MCC here
{
	TMachineSignalState sigstate;
	MachineSuspendSignals(&sigstate);
	int index;
	index = getIndex(thread);
	if(index < 0)
	{
		MachineResumeSignals(&sigstate);
		return VM_STATUS_ERROR_INVALID_ID;
	}
	if(threads[index]->state != VM_THREAD_STATE_DEAD)
	{
		MachineResumeSignals(&sigstate);
		return VM_STATUS_ERROR_INVALID_STATE;
	}

	TCB* temp;
	temp = threads[index];
	//temp = threads[thread];
	TVMThreadEntry threadEntry;
	threadEntry = &skeleton;
	//mcntxref will be saved in TCB
	//change state to ready and put in readyQ
	//pass in skeleton function with parameter TCB; called inside
	MachineContextCreate(temp->context, threadEntry, temp, temp->stack, temp->stacksize);

	if(temp->prio > threads[currentTID]->prio)
	{
		//if higher priority than current
		//put current to readyQ and switch to new
		TVMThreadID oldTID = currentTID;
		TVMThreadID newTID = temp->thid;

		threads[oldTID]->state = VM_THREAD_STATE_READY;
		//sleepQ.push_back(threads[oldTID]);
		if(threads[oldTID]->prio == VM_THREAD_PRIORITY_HIGH)
			readyQH.push(*(threads[oldTID]));
		else if(threads[oldTID]->prio == VM_THREAD_PRIORITY_NORMAL)
			readyQN.push(*(threads[oldTID]));
		else if(threads[oldTID]->prio == VM_THREAD_PRIORITY_LOW)
			readyQL.push(*(threads[oldTID]));
		else
			readyQB.push(*(threads[oldTID]));

		currentTID = newTID;
		temp->state = VM_THREAD_STATE_RUNNING;
		MachineContextSwitch(threads[oldTID]->context,threads[newTID]->context);
	}
	else
	{		
		temp->state = VM_THREAD_STATE_READY;
		if(temp->prio == VM_THREAD_PRIORITY_HIGH)
			readyQH.push(*temp);
		else if(temp->prio == VM_THREAD_PRIORITY_NORMAL)
			readyQN.push(*temp);
		else if(temp->prio == VM_THREAD_PRIORITY_LOW)
			readyQL.push(*temp);
		else
			readyQB.push(*temp);
	}
	MachineResumeSignals(&sigstate);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMThreadTerminate(TVMThreadID thread)//end a thread, release mutexes,trigger another thread.
{
	unsigned int i;

	int index;
	index = getIndex(thread);
	if(index < 0)
		return VM_STATUS_ERROR_INVALID_ID;
	if(threads[index]->state == VM_THREAD_STATE_DEAD)
		return VM_STATUS_ERROR_INVALID_STATE;
	
	threads[index]->state = VM_THREAD_STATE_DEAD;
	//threads[thread]->state = VM_THREAD_STATE_DEAD;

	for(i = 0; i < mutices.size(); i++)
	{
		if(mutices[i]->owner = thread)
		{
			mutices[i]->locked = false;
			mutices[i]->owner = -1;
		}
	}

	if(currentTID == 0)
		return VM_STATUS_SUCCESS;
	scheduler(1);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMThreadID(TVMThreadIDRef threadref)
{
	if(!threadref)
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	*threadref = currentTID;
	return VM_STATUS_SUCCESS;
}

TVMStatus VMThreadState(TVMThreadID thread, TVMThreadStateRef stateref)
{
	if(!stateref)
		return VM_STATUS_ERROR_INVALID_PARAMETER;

	int index;
	index = getIndex(thread);
	if(index < 0)
		return VM_STATUS_ERROR_INVALID_ID;
	//if(thread >= threads.size())
	//	return VM_STATUS_ERROR_INVALID_ID;

	//*stateref = threads[thread]->state;
	*stateref = threads[index]->state;
	return VM_STATUS_SUCCESS;
}

//update readyQ sleepQ constantly
//context switch at the end if necessary
void scheduler(int wherefrom)
{
	TMachineSignalState sigstate;
	MachineSuspendSignals(&sigstate);
	TVMThreadID oldTID  = currentTID;
	queue<TCB>* ptr;

	TCB *threadToRun;
	//if there is thread in the readyQ
	if(!readyQH.empty())
	{
		threadToRun = threads[readyQH.front().thid];
		ptr = &readyQH;
	}
	else if(!readyQN.empty())
	{
		threadToRun = threads[readyQN.front().thid];
		ptr = &readyQN;
	}
	else if(!readyQL.empty())
	{
		threadToRun = threads[readyQL.front().thid];
		ptr = &readyQL;
	}
	else
	{
		threadToRun = threads[readyQB.front().thid];
		ptr = &readyQB;
	}
	if(threadToRun)
	{
		//if nothing but idle thread in readyQ, run idle thread
		if (threadToRun->thid != 1)	
			ptr->pop();
		if(threadToRun->state != VM_THREAD_STATE_DEAD)
			threadToRun->state = VM_THREAD_STATE_RUNNING;
		currentTID = threadToRun->thid;
		//context switch
		MachineContextSwitch(threads[oldTID]->context,threadToRun->context);
		MachineResumeSignals(&sigstate);
	}
}

TVMStatus VMThreadSleep(TVMTick tick)
{
	if(tick ==VM_TIMEOUT_INFINITE)
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	if(tick == VM_TIMEOUT_IMMEDIATE )
		return VM_STATUS_SUCCESS;
	threads[currentTID]->state = VM_THREAD_STATE_WAITING;
	threads[currentTID]->sleepcount = tick;
	sleepQ.push_back(threads[currentTID]);

	scheduler(2);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMMutexCreate(TVMMutexIDRef mutexref)
{
	//MachineResumeSignals sigstate;
	if(!mutexref)
		return VM_STATUS_ERROR_INVALID_PARAMETER;

	mutex* newMutex = new mutex;
	
	newMutex->mid = mutices.size();
	newMutex->locked = false;
	newMutex->owner = currentTID;
	mutices.push_back(newMutex);

	*mutexref = newMutex->mid;
	//MachineResumeSignals(sigstate);
	return VM_STATUS_SUCCESS;
}

//delete mutex from mutex vector
TVMStatus VMMutexDelete(TVMMutexID mutex)
{
	if(mutex >= mutices.size())
		return VM_STATUS_ERROR_INVALID_ID;
	if(mutices[mutex]->locked)
		return VM_STATUS_ERROR_INVALID_STATE;
	mutices.erase(mutices.begin()+mutex); 
	return VM_STATUS_SUCCESS;
}

TVMStatus VMMutexQuery(TVMMutexID mutex, TVMThreadIDRef ownerref)
{
	if (ownerref == NULL)
		return VM_STATUS_ERROR_INVALID_PARAMETER;

	unsigned int i;
	for(i = 0; i < mutices.size(); i++)
	{
		//found mutex
		if(mutices[i]->mid == mutex)
		{
			if(!mutices[i]->locked)
				*ownerref = VM_THREAD_ID_INVALID;
			else
				*ownerref = mutices[i]->owner;
			return VM_STATUS_SUCCESS;
		}
	}
	return VM_STATUS_ERROR_INVALID_ID;
}

TVMStatus VMMutexAcquire(TVMMutexID mutex, TVMTick timeout)
{
	if(mutex >= mutices.size())
		return VM_STATUS_ERROR_INVALID_ID;
	if(timeout == VM_TIMEOUT_IMMEDIATE)
	{
		if(!mutices[mutex]->locked)
		{
			mutices[mutex]->owner = currentTID;
			mutices[mutex]->locked = true;
			return VM_STATUS_SUCCESS;
		}
		else
		{
			return VM_STATUS_FAILURE;
		}
	}

	if(timeout != VM_TIMEOUT_INFINITE)
	{
		unsigned int i = tickCount;
		while(1)
		{
			if(tickCount >= i+timeout) //check later!! potential loop
				return VM_STATUS_FAILURE;
			if(!mutices[mutex]->locked)
			{
				mutices[mutex]->owner = currentTID;
				mutices[mutex]->locked = true;
				//MachineResumeSignals(sigstate);
				return VM_STATUS_SUCCESS;
			}
		}
	}
	else//timeout infinite; need to block thread
	{
		if(!mutices[mutex]->locked)//unlocked, return
		{
			mutices[mutex]->owner = currentTID;
			mutices[mutex]->locked = true;
			//MachineResumeSignals(sigstate);
			return VM_STATUS_SUCCESS;
		}
		else//locked, block thread and push into mutex's waitingQ
		{
			threads[currentTID]->state = VM_THREAD_STATE_WAITING;
			mutices[mutex]->waitingQ.push(*(threads[currentTID]));
			sleepQ.push_back(threads[currentTID]);
			//MachineResumeSignals(sigstate);
			scheduler(7);
		}
	}
	return VM_STATUS_SUCCESS;
}

//change to unlocked
TVMStatus VMMutexRelease(TVMMutexID mutex)
{
	if(mutex >= mutices.size())
		return VM_STATUS_ERROR_INVALID_ID;
	unsigned int i;
	for(i = 0; i < mutices.size(); i++)
	{
		if(mutices[i]->mid == mutex)
		{
			if(mutices[i]->locked && mutices[i]->owner == currentTID)
			{
				mutices[i]->locked = false;
				return VM_STATUS_SUCCESS;
			}
			else
				return VM_STATUS_ERROR_INVALID_STATE;
		}
	}
	//mutex not found
	return VM_STATUS_ERROR_INVALID_ID;
}

void FileCallback(void *calldata, int result)
{		
		TCB* tempThread = (TCB*)calldata;
		TVMThreadID tempID = tempThread->thid;

		if(result >= 0)
		{
			if(tempThread->fd <= 0)
				tempThread->fd = result;
			tempThread->lengthRead += result;
			tempThread->lengthWriten += result;
			tempThread->offset = result;
			tempThread->leftToProcess--;
			//look up the thread to remove from sleepQ
			if(tempThread->leftToProcess == 0)
			{
				unsigned int i;
				for (i = 0; i < sleepQ.size(); i++)
				{
					if(sleepQ[i]->thid == tempID)
						break;
				}
				if(i == sleepQ.size())
					cout << "\nsleepQ item not found in callBack";

				sleepQ[i]->state = VM_THREAD_STATE_READY;
				if(sleepQ[i]->prio == VM_THREAD_PRIORITY_HIGH)
					readyQH.push(*(sleepQ[i]));
				else if(sleepQ[i]->prio == VM_THREAD_PRIORITY_NORMAL)
					readyQN.push(*(sleepQ[i]));
				else if(sleepQ[i]->prio == VM_THREAD_PRIORITY_LOW)
					readyQL.push(*(sleepQ[i]));
				else
					readyQB.push(*(sleepQ[i]));
				sleepQ.erase(sleepQ.begin()+ i);
			}
		}
		else
			printf("\nFile not processed successfully\n");
		return;
}

void FilePrintCallback(void *calldata, int result)
{		
	TCB* tempThread = (TCB*)calldata;
	TVMThreadID tempID = tempThread->thid;


	if(result >= 0)
	{
		//look up the thread to remove from sleepQ
		unsigned int i = 0;
		for (i = 0; i < sleepQ.size(); i++)
		{
			if(sleepQ[i]->thid == tempID)
				break;
		}
		if(i == sleepQ.size())
		{
			cout << "\nsleepQ item not found in callBack";
			return;
		}

		sleepQ[i]->state = VM_THREAD_STATE_READY;
		if(sleepQ[i]->prio == VM_THREAD_PRIORITY_HIGH)
			readyQH.push(*(sleepQ[i]));
		else if(sleepQ[i]->prio == VM_THREAD_PRIORITY_NORMAL)
			readyQN.push(*(sleepQ[i]));
		else if(sleepQ[i]->prio == VM_THREAD_PRIORITY_LOW)
			readyQL.push(*(sleepQ[i]));
		else
			readyQB.push(*(sleepQ[i]));
		sleepQ.erase(sleepQ.begin()+ i);
	}
	else
		printf("\nFile not processed successfully\n");
	return;
}

//will be in block state; open,write,read,seek and close uses the same fd
TVMStatus VMFileOpen(const char *filename, int flags, int mode, int *filedescriptor)
{
	threads[currentTID]->fd = 0;
	TMachineSignalState sigstate;
	MachineSuspendSignals(&sigstate);

	TMachineFileCallback openCallback;
	openCallback = &FileCallback;
	TVMThreadID thisTID = currentTID;

	//filedescriptor points TCB thread fd
	threads[currentTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[currentTID]);

	MachineFileOpen(filename, flags, mode, openCallback, threads[currentTID]);
	//threads[thisTID]->fd = filedescriptor; 
	MachineResumeSignals(&sigstate);
	scheduler(6);
	*filedescriptor = threads[thisTID]->fd;
	
	return VM_STATUS_SUCCESS;
}

//findSpaces in memory, 64 bytes per block; mark in freeList
//index parameter for later deletion
char* findSpace(TVMMemoryPoolID memory, int length, int* index)
{
	//cout << "memory size: " << length << endl;
	bool found = false;
	int i,j,k;
	MemPool* temp = poolList[memory];
	char* ptr;

	int blocksNeeded = length/64;
	if(length % 64 > 0)
		blocksNeeded++;
	//cout << "need: " << blocksNeeded << " blocks." << endl;
	//find consecutive free blocks
	for(i = 0; i < temp->blockNum; i++)
	{
		if(temp->isFree[i] < 0)
		{
			//cout << "finding: " << i << " is free!!" << endl;
			for(j = i; (j < blocksNeeded+i) && (temp->isFree[j] < 0); j++);
			if(j == blocksNeeded+i)
				{found = true;break;}//found space!
			else
				{i = j-1; continue;}//if not consecutive free, continue from j
		}
	}
	//if found space, return  blockIndex*64 + basePtr
	if(found)
	{
		//cout << "base for memory: " << memory << " " << static_cast<void*>(poolList[memory]->basePtr) << endl;
		ptr = temp->basePtr + i*64;
		//cout << "i: " << i << endl;
		//cout << "ptr: " << static_cast<void*>(ptr) << endl;
		//cout << "allocating num: "<< temp->allocateNum << endl;
		for(k = 0; k < blocksNeeded; k++)
			temp->isFree[k+i] = temp->allocateNum;
		temp->allocateNum++;
		*index = i;
	}
	else
	{
		//cout << "space not found!" << endl;
		*index = -1;
		ptr = NULL;
	}
	return ptr;
}

//free memory (in shared memory pool)
void freeSpace1(TVMMemoryPoolID memory, int length, int index)
{
	int i;
	int blocksNeeded = length/64;
	if(length % 64 > 0)
		blocksNeeded++;
	//cout << "freeing" << " blcoks needed: " << blocksNeeded << endl;
	for(i = 0; i < blocksNeeded; i++)
		poolList[memory]->isFree[index + i] = -1;
}

//free memory (in system memory pool)
//chunk(base + isFreeMarks) -> deallocate
void freeSpace2(TVMMemoryPoolID memory, char* base)
{
	int i;
	int index;
	int allocationGroup;
	//calculate index in freelist
	index = (base - poolList[memory]->basePtr)/64;
	allocationGroup = poolList[memory]->isFree[index];
	for(i = 0; poolList[memory]->isFree[index + i] == allocationGroup; i++)
	{
		//cout << "allocationGroup is : " << allocationGroup << endl;
		poolList[memory]->isFree[i + index] = -1;
	}	
}

TVMStatus VMFileWrite(int filedescriptor, void *data, int *length)
{
	threads[currentTID]->lengthWriten =0;
	int i;
	TMachineSignalState sigstate;
	MachineSuspendSignals(&sigstate);

	TMachineFileCallback writeCallback;
	writeCallback = &FileCallback;

	TMachineFileCallback printCallback;
	printCallback = &FilePrintCallback;

	threads[currentTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[currentTID]);

	//if data > 512 bytes, break and store info. in vectors
	//find spaces for each data in shared memory; mark freeList
	//set space to be free after completion
	vector<void*> dataList;
	vector<int> lengthList;
	vector<int> indexList;//used for freeing space later

	int writeCount = 0;
	if(*length < 512) //one write only
	{
		writeCount = 1;
		dataList.push_back(data);
		lengthList.push_back(*length);
		indexList.push_back(-1);
	}
	else //data larger than 512 bytes
	{
		writeCount = (*length) / 512;//512 bytes per write
		for (i = 0; i < writeCount; i++)
		{
			dataList.push_back(data+i*512);
			lengthList.push_back(512);
			indexList.push_back(-1);
		}
		if((*length % 512) > 0)//last write = remaining bytes
		{
			writeCount++;
			dataList.push_back(data+i*512);
			lengthList.push_back(512);
			indexList.push_back(-1);
		}
	}
	threads[currentTID]->leftToProcess = writeCount;

	char* tempPtr;
	for(i = 0; i < writeCount; i++)
	{
		//findspace return a pointer in memory pool 1
		//cout << "\nsharedmem pool base:" << static_cast<void*>(poolList[1]->basePtr) << endl;
		tempPtr = findSpace(1, lengthList[i], &indexList[i]);

		memcpy(tempPtr, dataList[i], (size_t)lengthList[i]);
		//cout << "tempPtr: " << static_cast<void*>(tempPtr) << endl;
		//cout << "length to write is: " << lengthList[i] << endl;
		//cout << "data to write is: " << (char*)dataList[i] << endl;
		if(filedescriptor != 1)	
			MachineFileWrite(filedescriptor, tempPtr, lengthList[i], writeCallback, threads[currentTID]);
		else
			MachineFileWrite(filedescriptor, tempPtr, lengthList[i], printCallback, threads[currentTID]);
	}
	MachineResumeSignals(&sigstate);
	scheduler(6);
	*length = threads[currentTID]->lengthWriten;
	for(i = 0; i < writeCount; i++)
		freeSpace1(1, lengthList[i], indexList[i]);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMFileClose(int filedescriptor)
{
	TMachineFileCallback closeCallback;
	closeCallback = &FileCallback;
	threads[currentTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[currentTID]);

	MachineFileClose(filedescriptor, closeCallback, threads[currentTID]);
	scheduler(6);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMFileRead(int filedescriptor, void *data, int *length)
{
	threads[currentTID]->lengthRead =0;
	TMachineFileCallback readCallback;
	readCallback = &FileCallback;
	threads[currentTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[currentTID]);

	MemPool* tempPool = poolList[1];
	MachineFileRead(filedescriptor, tempPool->basePtr, *length, readCallback, threads[currentTID]);
	scheduler(6);
	memcpy(data, tempPool->basePtr, (size_t)*length);//schedlued back from idle after callback
	*length = threads[currentTID]->lengthRead;
    return VM_STATUS_SUCCESS;
}

TVMStatus VMFileSeek(int filedescriptor, int offset, int whence, int *newoffset)
{
	TMachineFileCallback seekCallback;
	seekCallback = &FileCallback;
	TVMThreadID thisTID = currentTID;
	threads[thisTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[thisTID]);

	MachineFileSeek(filedescriptor, offset, whence, seekCallback, threads[currentTID]);
	scheduler(6);
	*newoffset = threads[currentTID]->offset;
	return VM_STATUS_SUCCESS;
}

}
