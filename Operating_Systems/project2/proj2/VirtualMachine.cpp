#include <vector>
#include <deque>
#include <queue>
#include <iostream>

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
#include <sys/types.h>
//#include <vector>
//using namespace std;

volatile int msPerTick;
volatile unsigned int tickCount; //increment each callback is returned
//volatile int sleepcount;
volatile int numThreads;
volatile TVMThreadID currentTID;


#define VM_THREAD_PRIORITY_BOTTOM                  ((TVMThreadPriority)0x00)

TVMMainEntry VMLoadModule(const char *module);
void VMUnloadModule(void);
TVMStatus VMFilePrint(int filedescriptor, const char *format, ...);
void scheduler(int wherefrom);
void printData(int filedescriptor, void *data, int *length);


struct TCB{
	TVMThreadEntry entry;
	void* param;
	SMachineContext* context;
    TVMThreadID thid;
    //TVMMutexID mid;
    volatile int sleepcount;
    volatile int ticksleft;
    void* stack;
    TVMMemorySize stacksize;
    TVMStatus state;
    bool printing;
    TVMThreadPriority prio;
    int* fd;
    int lengthRead;
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
priority_queue<TCB> readyQ;

struct mutex{
	volatile bool locked;
	TVMThreadID owner;
	priority_queue<TCB> waitingQ;
	TVMMutexID mid;
};

vector<mutex*> mutices;

//Check mutex vector
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

				//look up the thread
				//to remove from sleepQ; push to readyQ
				unsigned int k;
				for (k = 0; k < sleepQ.size(); k++)
				{
					if(sleepQ[k]->thid == tempID)
						break;
				}

				if(k == sleepQ.size())
					cout << "\nsleepQ item not found in callBack";

				sleepQ[k]->state = VM_THREAD_STATE_READY;
				readyQ.push(*(sleepQ[k]));
				sleepQ.erase(sleepQ.begin()+ k);
				MachineResumeSignals(&sigstate);
				scheduler(0);
			}
		}
	}
	//switch threads per tick
	//push current into  before checking sleepcount
	
	threads[currentTID]->state = VM_THREAD_STATE_READY;
	if (currentTID != 1)
		readyQ.push(*(threads[currentTID]));

	unsigned int i;
	for (i = 0; i < sleepQ.size(); i++)
	{
		sleepQ[i]->sleepcount--;
		if(sleepQ[i]->sleepcount == 0)
		{
			sleepQ[i]->state = VM_THREAD_STATE_READY;//change state to ready
			readyQ.push(*(sleepQ[i])); //push to readyQ
			sleepQ.erase(sleepQ.begin()+i);//pop from sleepQ
		}
	}
	MachineResumeSignals(&sigstate);
	scheduler(0);
	return;	
}

//while with scheduler
void idleThreadEntry(void *param)
{
	MachineEnableSignals();
	while(1);
}

//load module and run; 
TVMStatus VMStart(int tickms, int argc, char *argv[])
{
	////MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
	TVMMainEntry entry;
	tickCount = 0;
	msPerTick = tickms;	
	numThreads = 1;
	//MachineInitialize(); //defalt ms/tick is 100
	//MachineRequestAlarm(msPerTick*1000, Callback, NULL);//waiting for callback

	entry = VMLoadModule(argv[0]);

	//main thread info
	TCB* mainThread = new TCB;
	SMachineContextRef mainContext = new SMachineContext;
	MachineContextSave(mainContext); //has to be reference?!
	mainThread->context = mainContext;
	mainThread->thid = 0;
	mainThread->prio = VM_THREAD_PRIORITY_NORMAL;
	mainThread->state = VM_THREAD_STATE_RUNNING;
	mainThread->sleepcount = 0;
	//push main thread into TVB vector, id = 0
	threads.push_back(mainThread);
	currentTID = 0;
	//create idle thread
	TVMThreadID idleThreadID;
	TVMThreadEntry idleEntry;
	idleEntry = &idleThreadEntry;
	VMThreadCreate(idleEntry, NULL, 0x100000, VM_THREAD_PRIORITY_BOTTOM, &idleThreadID);
	MachineInitialize(); //defalt ms/tick is 100
        MachineRequestAlarm(msPerTick*1000, Callback, NULL);//waiting for callback
	VMThreadActivate(idleThreadID);

	//unsigned int i;
	//for (i = 0; i < threads.size(); i++)
	//	threads[i]->fd = -1;

	entry(argc, argv);

	//MachineResumeSignals(sigstate);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMTickMS(int *tickmsref)
{
	////MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
	*tickmsref = msPerTick;
	//MachineResumeSignals(sigstate);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMTickCount(TVMTickRef tickref)
{
	////MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
	*tickref = tickCount;

	//MachineResumeSignals(sigstate);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMThreadCreate(TVMThreadEntry entry, void *param, TVMMemorySize memsize, TVMThreadPriority prio, TVMThreadIDRef tid)
{
	////MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
	numThreads++;
	TCB* th = new TCB;

	th->stack = new uint8_t[memsize];
	th->context = new SMachineContext;
	th->stacksize = memsize;
	th->entry = entry;
	th->param = param;
	th->printing = false;
	th->ticksleft = 1;
	th->prio = prio;
	th->sleepcount = 0;
	*tid = numThreads-1;
	th->thid = *tid;
	th->state = VM_THREAD_STATE_DEAD;
	threads.push_back(th);

	//MachineResumeSignals(sigstate);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMThreadDelete(TVMThreadID thread)
{
	////MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
	threads.erase(threads.begin()+thread);

	//MachineResumeSignals(sigstate);
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
	TCB* temp;
	temp = threads[thread];
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
		readyQ.push(*(threads[oldTID]));
		currentTID = newTID;

		temp->state = VM_THREAD_STATE_RUNNING;
		MachineResumeSignals(&sigstate);
		MachineContextSwitch(threads[oldTID]->context,threads[newTID]->context);
	}
	else
	{
	
			
		temp->state = VM_THREAD_STATE_READY;
		readyQ.push(*temp);
	}
	MachineResumeSignals(&sigstate);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMThreadTerminate(TVMThreadID thread)//end a thread, release mutexes,trigger another thread.
{
	////MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);

	threads[thread]->state = VM_THREAD_STATE_DEAD;

	//MachineResumeSignals(sigstate);
	scheduler(1);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMThreadID(TVMThreadIDRef threadref)
{
	//MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
	*threadref = currentTID;

	//MachineResumeSignals(sigstate);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMThreadState(TVMThreadID thread, TVMThreadStateRef stateref)
{
	//MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
	*stateref = threads[thread]->state;

	//MachineResumeSignals(sigstate);
	return VM_STATUS_SUCCESS;
}

//update readyQ sleepQ constantly
//context switch at the end if necessary
void scheduler(int wherefrom)
{
	TMachineSignalState sigstate;
	MachineSuspendSignals(&sigstate);
	TVMThreadID oldTID  = currentTID;

	TCB *threadToRun;
	//if there is thread in the readyQ
	threadToRun = threads[readyQ.top().thid];
	cout << "threadToRun is: "<< threadToRun->thid << endl;
	
	vector<TCB> tempQ;
	while(readyQ.size()!=0)
	{
		cout << "readyQ item id: " << readyQ.top().thid << endl;
		tempQ.push_back(readyQ.top());
		readyQ.pop();
	}
	for(unsigned int i = 0; i < tempQ.size(); i++)
	{
		readyQ.push(tempQ[i]);
	}

	if(threadToRun)
	{
		//if nothing but idle thread in readyQ, run idle thread
		if (threadToRun->thid != 1)	
			readyQ.pop();//remove thread from readyQ
		cout << "readyQ size: " << readyQ.size() << endl;
		threadToRun->state = VM_THREAD_STATE_RUNNING;
		currentTID = threadToRun->thid;
		//context switch
		MachineResumeSignals(&sigstate);
		MachineContextSwitch(threads[oldTID]->context,threadToRun->context);
	}
}

TVMStatus VMThreadSleep(TVMTick tick)
{
	//MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
	//change state to wait, push into sleepQ
	threads[currentTID]->state = VM_THREAD_STATE_WAITING;
	threads[currentTID]->sleepcount = tick;
	sleepQ.push_back(threads[currentTID]);

	//MachineResumeSignals(sigstate);
	scheduler(2);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMMutexCreate(TVMMutexIDRef mutexref)
{
	//MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
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
	//MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);

	mutices.erase(mutices.begin()+mutex); 
	//MachineResumeSignals(sigstate);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMMutexQuery(TVMMutexID mutex, TVMThreadIDRef ownerref)
{
	//MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
	if (ownerref == NULL)
		return VM_STATUS_ERROR_INVALID_PARAMETER;

	unsigned int i;
	for(i = 0; i < mutices.size(); i++)
	{
		//found mutex
		if(mutices[i]->mid == mutex)
		{
			if(mutices[i]->locked)
			{
				*ownerref = mutices[i]->owner;
				return VM_STATUS_SUCCESS;
			}
			else//no owner, unlocked case
				return VM_THREAD_ID_INVALID;
		}
	}
	//MachineResumeSignals(sigstate);
	//mutex not found
	return VM_STATUS_ERROR_INVALID_ID;
}



//if timeout is immediate, check mutex vector, if is locked, return
//else, 
//		if timeout < infinity; while(timeout), wait till it unlock; return failure if time is out
//		if timeout == infinity; if mutex is locked;block current tread, push current thread into mutex's waitingQ
//								if mutex is unlocked, change owner, change to locked and return success
TVMStatus VMMutexAcquire(TVMMutexID mutex, TVMTick timeout)
{
	//MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
	if(timeout == VM_TIMEOUT_IMMEDIATE)
	{
		if(!mutices[mutex]->locked)
		{//
			mutices[mutex]->owner = currentTID;
			mutices[mutex]->locked = true;
			//MachineResumeSignals(sigstate);	
			return VM_STATUS_SUCCESS;
		}
		else
		{
			//MachineResumeSignals(sigstate);
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
	//MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);
	unsigned int i;
	for(i = 0; i < mutices.size(); i++)
	{
		//found mutex
		if(mutices[i]->mid == mutex)
		{
			if(mutices[i]->locked && mutices[i]->owner == currentTID)
			{
				mutices[i]->locked = false;
				//MachineResumeSignals(sigstate);
				return VM_STATUS_SUCCESS;
			}
			else//no owner, unlocked case
			{
				//MachineResumeSignals(sigstate);
				return VM_THREAD_ID_INVALID;
			}
		}
	}
	//MachineResumeSignals(sigstate);
	//mutex not found
	return VM_STATUS_ERROR_INVALID_ID;
}


//calldata is the new fd
void FileCallback(void *calldata, int result)
{		
	////MachineResumeSignals sigstate;
	//////MachineResumeSignals(sigstate);
	        cout << "FileCallback result: " << result << endl;
		TCB* tempThread = (TCB*)calldata;
		TVMThreadID tempID = tempThread->thid;

		if(result >= 0)
		{
			if(*(tempThread->fd) <= 0)
				*(tempThread->fd) = result;

			tempThread->lengthRead = result;
			tempThread->offset = result;

			//look up the thread to remove from sleepQ
			unsigned int i;
			for (i = 0; i < sleepQ.size(); i++)
			{
				if(sleepQ[i]->thid == tempID)
					break;
			}
			if(i == sleepQ.size())
				cout << "\nsleepQ item not found in callBack";

			sleepQ[i]->state = VM_THREAD_STATE_READY;
			readyQ.push(*(sleepQ[i]));
			sleepQ.erase(sleepQ.begin()+ i);
			//cout << "readyQ top is: " << readyQ.top().thid << endl;
			////MachineResumeSignals(sigstate);
			//scheduler(5);
		}
		else
			printf("\nFile not processed successfully\n");
		return;
}

void FilePrintCallback(void *calldata, int result)
{		
	////MachineResumeSignals sigstate;
	//////MachineResumeSignals(sigstate);
		TCB* tempThread = (TCB*)calldata;
		TVMThreadID tempID = tempThread->thid;

		if(result >= 0)
		{
			//look up the thread to remove from sleepQ
			unsigned int i;
			for (i = 0; i < sleepQ.size(); i++)
			{
				if(sleepQ[i]->thid == tempID)
					break;
			}
			if(i == sleepQ.size())
				cout << "\nsleepQ item not found in callBack";

			sleepQ[i]->state = VM_THREAD_STATE_READY;
			readyQ.push(*(sleepQ[i]));
			sleepQ.erase(sleepQ.begin()+ i);
			//cout << "readyQ top is: " << readyQ.top().thid << endl;
			////MachineResumeSignals(sigstate);
			//scheduler(5);
		}
		else
			printf("\nFile not processed successfully\n");
		return;
}


//will be in block state; open,write,read,seek and close uses the same fd
TVMStatus VMFileOpen(const char *filename, int flags, int mode, int *filedescriptor)
{
	TMachineSignalState sigstate;
	MachineSuspendSignals(&sigstate);

	TMachineFileCallback openCallback;
	openCallback = &FileCallback;
	TVMThreadID thisTID = currentTID;

	//filedescriptor points TCB thread fd
	threads[currentTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[currentTID]);

	MachineFileOpen(filename, flags, mode, openCallback, threads[currentTID]);
//	*filedescriptor = (threads[thisTID]->fd);
	threads[thisTID]->fd = filedescriptor; 
	MachineResumeSignals(&sigstate);
	scheduler(6);
	
	return VM_STATUS_SUCCESS;
}


TVMStatus VMFileWrite(int filedescriptor, void *data, int *length)
{
	TMachineSignalState sigstate;
	MachineSuspendSignals(&sigstate);

	TMachineFileCallback writeCallback;
	writeCallback = &FileCallback;

	TMachineFileCallback printCallback;
	printCallback = &FilePrintCallback;

	threads[currentTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[currentTID]);

	if(filedescriptor != 1)	
		MachineFileWrite(filedescriptor, data, *length, writeCallback, threads[currentTID]);
	else
		MachineFileWrite(filedescriptor, data, *length, printCallback, threads[currentTID]);

	MachineResumeSignals(&sigstate);
	scheduler(6);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMFileClose(int filedescriptor)
{
	//MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);

	TMachineFileCallback closeCallback;
	closeCallback = &FileCallback;
	//if(filedescriptor != threads[currentTID]->fd)
	//	printf("\nfiledescriptors not matching, check threads!!!\n");
	threads[currentTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[currentTID]);

	MachineFileClose(filedescriptor, closeCallback, threads[currentTID]);

	//MachineResumeSignals(sigstate);
	scheduler(6);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMFileRead(int filedescriptor, void *data, int *length)
{
	//MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);

	TMachineFileCallback readCallback;
	readCallback = &FileCallback;
	//if(filedescriptor != threads[currentTID]->fd)
	//	printf("\nfiledescriptors not matching, check threads!!!\n");
	threads[currentTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[currentTID]);

	MachineFileRead(filedescriptor, data, *length, readCallback, threads[currentTID]);
	*length = threads[currentTID]->lengthRead;

	//MachineResumeSignals(sigstate);
	scheduler(6);
    return VM_STATUS_SUCCESS;
}

TVMStatus VMFileSeek(int filedescriptor, int offset, int whence, int *newoffset)
{
	//MachineResumeSignals sigstate;
	////MachineResumeSignals(sigstate);

	TMachineFileCallback seekCallback;
	seekCallback = &FileCallback;
	//if(filedescriptor != threads[currentTID]->fd)
	//	printf("\nfiledescriptors not matching, check threads!!!\n");	
	TVMThreadID thisTID = currentTID;
	threads[thisTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[thisTID]);

	MachineFileSeek(filedescriptor, offset, whence, seekCallback, threads[currentTID]);
	*newoffset = threads[currentTID]->offset;
	//MachineResumeSignals(sigstate);
	scheduler(6);
	return VM_STATUS_SUCCESS;
}

void printData(int filedescriptor, void *data, int *length)
{
	write(filedescriptor, data, *length);
}

}
