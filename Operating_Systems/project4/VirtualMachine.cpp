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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
void FileCallback(void *calldata, int result);
void VMUnloadModule(void);
void Mount(const char *mount);
TVMStatus VMFilePrint(int filedescriptor, const char *format, ...);
void scheduler(int wherefrom);
void freeSpace2(TVMMemoryPoolID memory, char* base);
void freeSpace1(TVMMemoryPoolID memory, int length, int index);
char* findSpace(TVMMemoryPoolID memory, int length, int* index);
void printInfo(SVMDirectoryEntry DirectoryEntry);
void readRootEntries();
void readBPB(const char *mount);
void readFAT();


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
    int index;
    vector<int>* lengthList;
    vector<int>* indexList;
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

struct BPB{
	char BS_OEMName[9];
	int BPB_BytsPerSec;
	int BPB_SecPerClus;
	int BPB_RsvdSecCnt;
	int BPB_NumFATs;
	int BPB_RootEntCnt;
	int BPB_TotSec16;
	int BPB_FATSz16;
	int BPB_HiddSec;
	int BPB_TotSec32;
	int BS_DrvNum;
	int BS_Reserved1;
	int BS_BootSig;
	int BS_VolID;
	char BS_VolLab[12];
	char BS_FilSysType[9];
	int FirstRootSector;
	int FirstDataSector;
	int RootDirectorySectors;
	int ClusterCount;
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

BPB* bpb = new BPB;

struct FCB
{	char name[VM_FILE_SYSTEM_SFN_SIZE];
	SVMDirectoryEntry *entry;
	vector<SVMDirectoryEntry*> entries;
	vector<int> clusterList;
	int firstCluster;
	int dd; 
	int bytesAccessed;//for file
	char data[100000];
	int entryCounter; //for directory
	bool isFile; 
	int permission; //7 rwx, 6 rw-, 5 r-x, 4 r--, 3 -wx, 2 -w-, 1 --x
	int size; // in bytes
};

vector<uint16_t*> FAT;
FCB *currentDir;
FCB *lastDir;
vector<FCB*> openedDir;
vector<SVMDirectoryEntry*> rootEntries;
vector<int> rootClusters;

void checkEntries()
{
	for(int i = 0; i < rootEntries.size();i++)
		printInfo(*(rootEntries[i]));
}

//load module and run; 
TVMStatus VMStart(int tickms, TVMMemorySize heapsize, TVMMemorySize sharedsize, const char *mount, int argc, char *argv[])
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
    MachineRequestAlarm(msPerTick*100, Callback, NULL);//universal callback

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
	Mount(mount);


	entry(argc, argv);
	return VM_STATUS_SUCCESS;
}

void setDateTime(SVMDateTimeRef DateTime, uint16_t date, uint16_t time1)
{
	unsigned int DYear;
    unsigned char DMonth;
    unsigned char DDay;
    unsigned char DHour;
    unsigned char DMinute;
    unsigned char DSecond;
    DYear = date >> 9;
    DYear += 1980; //modify year to 1980+offset
    DMonth = (date >> 5) & 0xF;
    DDay = date & 0x1F;
    DHour = time1 >> 11;
    DMinute = (time1 >> 5) & 0x3F;
    DSecond = time1 & 0x1F;
    DSecond << 1; //multiple second by 2
    DateTime->DYear = DYear;
    DateTime->DMonth = DMonth;
    DateTime->DDay = DDay;
    DateTime->DHour = DHour;
    DateTime->DMinute = DMinute;
    DateTime->DSecond = DSecond;
    //cout << "date: "<< endl;
    //cout << "year is: " << DYear<< endl;
    //cout << "month is: " << DMonth << endl;
}

int filedescriptorInit;

bool invalidName(char *ptr)
{
	int i;
	if(ptr[0] == 0x20)
		return true;
	for(i = 0; i < 11;i++)
	{
		if(ptr[i] < 0x20 && ptr[0] != 0x05)
			return true;
		if(ptr[i] == 0x22 || ptr[i] == 0x2A || ptr[i] == 0x2B || ptr[i] == 0x2C || ptr[i] == 0x2E || ptr[i] == 0x2F || ptr[i] == 0x3A || ptr[i] == 0x3B || ptr[i] == 0x3C||ptr[i] == 0x3D||ptr[i] == 0x3E ||ptr[i] == 0x3F|| ptr[i] == 0x5B || ptr[i] == 0x5C || ptr[i] == 0x5D || ptr[i] ==0x7C)
			return true;
	}
	return false;
}

void Mount(const char *mount)
{
	
	//TMachineFileCallback seekCallback = &FileCallback;

	readBPB(mount);
	//read FAT section into FAT (linked list of 4 byte cluster label)
	readFAT();
	//read root entries 
	readRootEntries();
}

void readFAT()
{
	int i, j;
	char *sector = new char[512];
	int length = 512;
	TMachineFileCallback openCallback = &FileCallback;
	TMachineFileCallback readCallback = &FileCallback;
	TMachineFileCallback seekCallback = &FileCallback;
	MemPool* tempPool = poolList[1];
	
	//from cluster 2 to cluster BPB_FATSz16 + 2
	for(i = 0; i < bpb->BPB_FATSz16; i++)
	{
		threads[currentTID]->offset = 0;
		MachineFileSeek(filedescriptorInit, bpb->BPB_RsvdSecCnt*512 + i*512, 0, seekCallback, threads[currentTID]);
		threads[currentTID]->offset = 0;
		threads[currentTID]->lengthRead = 0;
		MachineFileRead(filedescriptorInit, tempPool->basePtr, length, readCallback, threads[currentTID]);
		while(threads[currentTID]->lengthRead <= 0);
		threads[currentTID]->lengthRead = 0;
		memcpy(sector, tempPool->basePtr, length);
		//256 entries per sector, 2 bytes per label
		//store all 4352 entries in a vector
		for (j = 0; j < 256; j++)
		{
			uint16_t* temp_16 = new uint16_t;
			memcpy(temp_16, sector+j*2, 2);
			FAT.push_back(temp_16);
		}
		//freeSpace1(1, 512, 0);
	}
}

void readBPB(const char *mount)
{
	char *sector = new char[512];
	int length = 512;
	TMachineFileCallback openCallback = &FileCallback;
	TMachineFileCallback readCallback = &FileCallback;
	TMachineFileCallback seekCallback = &FileCallback;
	MemPool* tempPool = poolList[1];

	threads[currentTID]->fd = -1;
	MachineFileOpen(mount, 0600, O_RDWR, openCallback, threads[currentTID]);
	while(threads[currentTID]->fd < 0);
	filedescriptorInit = threads[currentTID]->fd;
	threads[currentTID]->fd = -1;

	threads[currentTID]->lengthRead = 0;
	MachineFileRead(filedescriptorInit, tempPool->basePtr, length, readCallback, threads[currentTID]);
	while(threads[currentTID]->lengthRead <= 0);
	threads[currentTID]->lengthRead = 0;
	memcpy(sector, tempPool->basePtr, length);
	//freeSpace1(1, 512, 0);

	memcpy(bpb->BS_OEMName, sector + 3, 8);
	bpb->BS_OEMName[8] = '\0';
	bpb->BPB_BytsPerSec = *(uint16_t *)(sector +11);
	bpb->BPB_SecPerClus = *(uint8_t *)(sector +13);
	bpb->BPB_RsvdSecCnt = *(uint16_t *)(sector +14);
	bpb->BPB_NumFATs = *(uint8_t *)(sector +16);
	bpb->BPB_RootEntCnt = *(uint16_t *)(sector +17);
	bpb->BPB_TotSec16 = *(uint16_t *)(sector +19);
	bpb->BPB_FATSz16 = *(uint16_t *)(sector +22);
	bpb->BPB_HiddSec = *(uint32_t *)(sector +28);
	bpb->BPB_TotSec32 = *(uint32_t *)(sector +32);
	bpb->BS_DrvNum = *(uint8_t *)(sector +36);
	bpb->BS_Reserved1 = *(uint8_t *)(sector +37);
	bpb->BS_BootSig = *(uint8_t *)(sector +38);
	bpb->BS_VolID = *(uint32_t *)(sector +39);
	memcpy(bpb->BS_VolLab, sector + 43, 11);
	bpb->BS_VolLab[11] = '\0';
	memcpy(bpb->BS_FilSysType, sector + 54, 8);
	bpb->BS_FilSysType[8] = '\0';
	//FirstSectorofCluster = ((N – 2) * bpb->BPB_SecPerClus) + bpb->FirstDataSector;
	
	bpb->FirstRootSector = bpb->BPB_RsvdSecCnt + bpb->BPB_NumFATs * bpb->BPB_FATSz16;
	//at 35
	bpb->RootDirectorySectors = (bpb->BPB_RootEntCnt * 32) / 512;
	//# = 32 sectors in bpb
	bpb->FirstDataSector = bpb->FirstRootSector + bpb->RootDirectorySectors;
	//at 67
	bpb->ClusterCount = (bpb->BPB_TotSec32 - bpb->FirstDataSector) / bpb->BPB_SecPerClus;
	//# clusters = 4096

}

void readRootEntries()
{
	int i, j;
	char *sector = new char[512];
	int length = 512;
	TMachineFileCallback openCallback = &FileCallback;
	TMachineFileCallback readCallback = &FileCallback;
	TMachineFileCallback seekCallback = &FileCallback;
	MemPool* tempPool = poolList[1];
	bool out = false;
	//FCB* tempFCB = new FCB;
	uint16_t tempTime, tempDate;
	//32 sectors
	for (i = 0; i < bpb->RootDirectorySectors; i++)
	{
		if(out == true)
			break;
		threads[currentTID]->lengthRead = 0;
		threads[currentTID]->offset = 0;
		//cout << "seekOffset should be: " << (bpb->FirstRootSector*512)+(i*512) << endl;
		MachineFileSeek(filedescriptorInit,(bpb->FirstRootSector*512)+(i*512), 0, seekCallback, threads[currentTID]);
		threads[currentTID]->offset = 0;
		MachineFileRead(filedescriptorInit, tempPool->basePtr, 512, readCallback, threads[currentTID]);
		while(threads[currentTID]->lengthRead <= 0);
		threads[currentTID]->lengthRead = 0;
		memcpy(sector, tempPool->basePtr, 512);
		//freeSpace1(1, 512, 0);

		uint8_t longCheck;
		for(j = 0; j < bpb->BPB_BytsPerSec / 32; j++)
		{
			SVMDirectoryEntry *tempEntry;
			VMMemoryPoolAllocate(0, 32, (void**)&(tempEntry));
			char *ptr = sector + j*32;
			longCheck = *(uint8_t *)(ptr + 11);
			if(longCheck == (0x01|0x02|0x04|0x08))
			{
				//cout << "this is a long name" << endl;
				continue;
			}
			if(*ptr == 0xE5)
			{
				cout << "empty directory" << endl;
				continue;
			}
			if (*ptr == 0x00) //no other following
			{
				out = true;
				break;
			}
			if(invalidName(ptr))
				continue;
			//cout << "offset: " << j*32 << endl;
			char n1[9];
			char n2[4];
			//memcpy(tempEntry->DShortFileName, ptr + 0, 11);
			tempEntry->DShortFileName[11] = '\0';
			//strcpy(tempEntry->DLongFileName, "not readyyyy\0");
			memcpy(n1, ptr + 0, 8);
			n1[8] = '\0';
			memcpy(n2, ptr + 8, 3);
			n2[3] = '\0';
			int count = 0;
			for(int k = 0; k < 8; k++)
			{	
				if(n1[k] == ' ')
					break;
				count++;
			}
			n1[count] = '\0';
			count = 0;
			for(int k = 0; k < 3; k++)
			{	
				if(n2[k] == ' ')
					break;
				count++;
			}
			n2[count] = '\0';

			strcat(tempEntry->DShortFileName, n1);
			if(n2[0] != '\0')
				strcat(tempEntry->DShortFileName, ".");
			strcat(tempEntry->DShortFileName, n2);
			cout << "shortName: "<< tempEntry->DShortFileName << endl;
			tempEntry->DAttributes = *(uint8_t *)(ptr + 11);
			tempEntry->DSize = *(uint32_t *)(ptr + 28);

			tempTime = *(uint16_t *)(ptr + 14);
			tempDate = *(uint16_t *)(ptr + 16);
			setDateTime(&tempEntry->DCreate, tempDate, tempTime);

			tempDate = *(uint16_t *)(ptr + 18);
			setDateTime(&tempEntry->DAccess, tempDate, tempTime);

			tempTime = *(uint16_t *)(ptr + 22);
			tempDate = *(uint16_t *)(ptr + 24);
			rootClusters.push_back(*(uint16_t *)(ptr + 26));
			setDateTime(&tempEntry->DModify, tempDate, tempTime);
			rootEntries.push_back(tempEntry);
			//printInfo(*tempEntry);
		}
	}
}

void printInfo(SVMDirectoryEntry DirectoryEntry)
{
	int Mil, Kil, One;
	VMPrint("%04d/%02d/%02d %02d:%02d %s ",DirectoryEntry.DModify.DYear, DirectoryEntry.DModify.DMonth, DirectoryEntry.DModify.DDay, (DirectoryEntry.DModify.DHour % 12) ? (DirectoryEntry.DModify.DHour % 12) : 12 , DirectoryEntry.DModify.DMinute, DirectoryEntry.DModify.DHour >= 12 ? "PM" : "AM");
    VMPrint("%s ", DirectoryEntry.DAttributes & VM_FILE_SYSTEM_ATTR_DIRECTORY ? "<DIR> " : "<FILE>");
    Mil = DirectoryEntry.DSize / 1000000;
    Kil = (DirectoryEntry.DSize / 1000) % 1000;
    One = DirectoryEntry.DSize % 1000;
    if(Mil){
        VMPrint("%3d,%03d,%03d ",Mil, Kil, One);   
    }
    else if(Kil){
        VMPrint("    %3d,%03d ", Kil, One);
    }
    else if(0 == (DirectoryEntry.DAttributes & VM_FILE_SYSTEM_ATTR_DIRECTORY)){
        VMPrint("        %3d ",One);
    }
    else{
        VMPrint("            ");   
    }
    VMPrint("%-13s %s\n",DirectoryEntry.DShortFileName, DirectoryEntry.DLongFileName);
}


TVMStatus VMDirectoryCurrent(char *abspath)
{
	//checkEntries();
	if(openedDir.size()== 0)
	{
		abspath[0] = '/';
		abspath[1] = '\0';
	}
	return VM_STATUS_SUCCESS;
}

TVMStatus VMDirectoryOpen(const char *dirname, int *dirdescriptor)
{
	//here, need to do search of dirname
	//assign dd and add FCB to openedFile list
	//checkEntries();
	FCB* tempFCB = new FCB;

	strcpy(tempFCB->name, "root");
	tempFCB->dd = openedDir.size();
	tempFCB->isFile = false;
	tempFCB->entryCounter = 0;
	tempFCB->size = 32*512;
	for(unsigned int i = 0; i < rootEntries.size(); i++)
	{
		SVMDirectoryEntry *tempEntry = new SVMDirectoryEntry;
		tempEntry = rootEntries[i];
		(tempFCB->entries).push_back(tempEntry);
		//printInfo(*tempFCB->entries.back());
	}
	currentDir = tempFCB;
	openedDir.push_back(tempFCB);
	*dirdescriptor = tempFCB->dd;
	return VM_STATUS_SUCCESS;
}

int getIndex3(int dirdescriptor)
{
	int index = -1;
	unsigned int i;
	for(i = 0; i < openedDir.size();i++)
	{
		if(openedDir[i]->dd == dirdescriptor)
		{
			index = i;
			return index;
		}
	}
	return index;
}

//only root now
//read in one 32 byte entry
TVMStatus VMDirectoryRead(int dirdescriptor, SVMDirectoryEntryRef dirent)
{
	FCB *temp = new FCB;
	int index;

	index = getIndex3(dirdescriptor);

	temp = openedDir[index];//get opened FCB
	if(temp->entryCounter >= temp->entries.size())
		return VM_STATUS_FAILURE;
	*dirent = *(temp->entries[temp->entryCounter]);
	(temp->entryCounter)++;
	//MemPool* tempPool = poolList[1];
	//char *sector = new char[512];
	//TMachineFileCallback readCallback = &FileCallback;
	//TMachineFileCallback seekCallback = &FileCallback;
	/*
	if (dirdescriptor == 0)
		cout << "reading root" << endl;
	//SVMDirectoryEntry *tempEntry = new SVMDirectoryEntry;
	uint16_t tempTime, tempDate;
	bool out = false;

	int index = getIndex3(dirdescriptor);
	if(index < 0)
		cout << "dirdescriptor not found in openedDir" << endl;
	
	while(1)
	{
		if(openedDir[index]->currentEntry >= openedDir[index]->sizeInSector * 16)
		{
			cout << "end of directory" << endl;
			return VM_STATUS_FAILURE;
		}
	
		int i = (openedDir[index]->currentEntry * 32) / 512; //which sector
		int j = (openedDir[index]->currentEntry * 32) % 512;//which entry offset in sector i (bytes)
		cout << "\ni: "<< i<< endl;
		cout << "j: "<< j << endl;
		threads[currentTID]->lengthRead = 0;
		threads[currentTID]->offset = 0;
		cout << "\n\nseekOffset should be: " << (bpb->FirstRootSector*512)+(i*512) << endl;
		MachineFileSeek(filedescriptorInit,(bpb->FirstRootSector*512)+(i*512), 0, seekCallback, threads[currentTID]);
		threads[currentTID]->offset = 0;

		MachineFileRead(filedescriptorInit, tempPool->basePtr, 512, readCallback, threads[currentTID]);
		while(threads[currentTID]->lengthRead <= 0);
		threads[currentTID]->lengthRead = 0;
		memcpy(sector, tempPool->basePtr, 512);
		freeSpace1(1, 512, 0);

		uint8_t longCheck;
		char *ptr = sector + j;
		longCheck = *(uint8_t *)(ptr + 11);
		if(longCheck | 0xFE == 0 || longCheck | 0xFD == 0 || longCheck | 0xFB == 0 || longCheck | 0xF7 == 0)
		{
			openedDir[index]->currentEntry++;
			cout << "this is a long name" << endl;
			continue;
		}
		if(*ptr == 0xE5|| *ptr == 0x05)
		{
			openedDir[index]->currentEntry++;
			cout << "empty directory" << endl;
			continue;
		}
		if ( *ptr == 0x00 ) //no other following
			return VM_STATUS_FAILURE;
		//cout << "offset: " << j*32 << endl;
		memcpy(tempEntry->DShortFileName, ptr + 0, 11);
		tempEntry->DShortFileName[11] = '\0';
		cout << "shortName: "<< tempEntry->DShortFileName << endl;
		tempEntry->DAttributes = *(uint8_t *)(ptr + 11);
		tempEntry->DSize = *(uint32_t *)(ptr + 28);

		tempTime = *(uint16_t *)(ptr + 14);
		tempDate = *(uint16_t *)(ptr + 16);
		setDateTime(&tempEntry->DCreate, tempDate, tempTime);

		tempDate = *(uint16_t *)(ptr + 18);
		setDateTime(&tempEntry->DAccess, tempDate, tempTime);

		tempTime = *(uint16_t *)(ptr + 22);
		tempDate = *(uint16_t *)(ptr + 24);
		setDateTime(&tempEntry->DModify, tempDate, tempTime);

		openedDir[index]->currentEntry++;
		dirent = tempEntry;
		return VM_STATUS_SUCCESS;
	}
	*/
	return VM_STATUS_SUCCESS;
}

TVMStatus VMDirectoryClose(int dirdescriptor)
{
	int index = getIndex3(dirdescriptor);
	if(index < 0)
		cout << "dirdescriptor not found in openedDir" << endl;
	openedDir.erase(openedDir.begin()+index);
	return VM_STATUS_SUCCESS;	
}

TVMStatus VMDirectoryChange(const char *path)
{
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
	//cout << "threadToRun : " << threadToRun->thid << endl;
	//cout << "readyQN isze: "<< readyQN.size() << endl;
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
	{
		scheduler(2);
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}
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
	//cout << "thread " << currentTID << " acquiring: " << mutex << endl;
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
			if(threads[currentTID]->index != 0)
			{
				int j = threads[currentTID]->index;
				freeSpace1(1,(*(threads[currentTID]->lengthList))[j], (*(threads[currentTID]->indexList))[j]);
				threads[currentTID]->index == 0;
			}
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
			if(sleepQ.size() == 0)
				return;
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
		if(threads[currentTID]->index != 0)
		{
			int j = threads[currentTID]->index;
			freeSpace1(1,(*(threads[currentTID]->lengthList))[j], (*(threads[currentTID]->indexList))[j]);
			threads[currentTID]->index == 0;
		}
		tempThread->lengthWriten += result;
		tempThread->leftToProcess--;
		//look up the thread to remove from sleepQ
		if(tempThread->leftToProcess == 0)
		{
			unsigned int i = 0;
			for (i = 0; i < sleepQ.size(); i++)
			{
				if(sleepQ[i]->thid == tempID)
					break;
			}
			if(sleepQ.size() == 0)
				return;
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
	}
	else
		printf("\nFile not processed successfully\n");
	return;
}

int findCluster()
{
	for(unsigned int i = 2; i < FAT.size(); i++)
	{
		if(*FAT[i] == 0)
		{
			*FAT[i] == 65535;
			cout << "found cluster at index: " << i << endl;
			return i;
		}
	}
	cout << "fail to find a cluster in FAT" << endl;
}

void combineData(int dirdescriptor)
{
	int index= getIndex3(dirdescriptor);
	int sectorCount;
	FCB *tempFCB = new FCB;
	tempFCB = openedDir[index];

	int i, j;
	char *sector = new char[513];
	int length = 512;
	int lengthSaved = 0;
	TMachineFileCallback readCallback = &FileCallback;
	TMachineFileCallback seekCallback = &FileCallback;
	MemPool* tempPool = poolList[1];
	bool out = false;
	int lastSector = 0;
	//cout << "entry size: " << tempFCB->entry->DSize;
	if (tempFCB->entry->DSize % 1024 > 512)
		lastSector = 1;
	//cout << "start to combine" << endl;
	//combine data clusters
	for (i = 0; i < tempFCB->clusterList.size(); i++)
	{
		for(j = 0; j < 2; j++)
		{
			//cout << "j: " << j << endl;
			if(i == tempFCB->clusterList.size() - 1)
			{
				if(j == lastSector)
				{
					out = true;
					length = tempFCB->entry->DSize % 512;
				}

			}
			sectorCount = j + (tempFCB->clusterList[i] - 2) * bpb->BPB_SecPerClus + bpb->FirstDataSector;
			threads[currentTID]->lengthRead = 0;
			threads[currentTID]->offset = 0;
			//cout << "seekOffset should be: " << (bpb->FirstRootSector*512)+(i*512) << endl;
			MachineFileSeek(filedescriptorInit, sectorCount*512, 0, seekCallback, threads[currentTID]);
			threads[currentTID]->offset = 0;
			MachineFileRead(filedescriptorInit, tempPool->basePtr + i * 1024 + j*512, length, readCallback, threads[currentTID]);
			cout << "i:" << i << "j: " << j << ",readLength: "<< threads[currentTID]->lengthRead << endl;

			while(threads[currentTID]->lengthRead <= 0) {
				//cout << "i:" << i << "j: " << j << ",readLength: "<< threads[currentTID]->lengthRead << endl;
				;
			}
			threads[currentTID]->lengthRead = 0;
			//memcpy(tempFCB->data + lengthSaved, tempPool->basePtr, length);
			//lengthSaved+=length;
			//sector[length] = '\0';
			//cout << "sector: \n" << sector << endl; 
			//memcpy(tempFCB->data + lengthSaved, sector, length);
			//strcat(tempFCB->data, sector);
//			cout << "\n\nsector" << sector << endl;
			if (out)
				break;
		}
	}
	memcpy(tempFCB->data, tempPool->basePtr, tempFCB->entry->DSize);
	tempFCB->data[tempFCB->entry->DSize] = '\0';
	cout << "\ndata: \n" << tempFCB->data << endl;
	cout << "\n\n\n\n\n";
	//cout << "\ndata size: "<< strlen(tempFCB->data) << endl;
}

//will be in block state; open,write,read,seek and close uses the same fd
TVMStatus VMFileOpen(const char *filename, int flags, int mode, int *filedescriptor)
{
	int dis;
	cout << "opening" << endl;
	VMDirectoryOpen("\\", &dis);
	bool create = false;
	//create new TCB ; add to openedDir
	FCB *tempFCB = new FCB;
	
	if(flags & O_CREAT == 0)
	{
		create = true;
		tempFCB->permission = 6;
	}
	else
	{
		if((flags & O_RDONLY) != 0)
			tempFCB->permission = 4;
		else if ((flags & O_WRONLY) != 0)
			tempFCB->permission = 2;
		else if((flags & O_RDWR) != 0)
			tempFCB->permission = 6;
	}
	
	
	tempFCB->isFile = true;
	tempFCB->size = 0;
	tempFCB->dd = openedDir.size()+2;

	tempFCB->bytesAccessed = 0;

	*filedescriptor = tempFCB->dd;
	//cout << "openedDir size: " << openedDir.size() << endl;
	int i;
	for (i = 0; i < openedDir.back()->entries.size(); i++)
	{
		if(strcmp(openedDir.back()->entries[i]->DShortFileName, filename) == 0)
		{
			cout << "found matching file" << endl;
			if(create) 
			{
				cout << "cannot create existing file";
				return VM_STATUS_FAILURE;
			}
			//assign entry	
			tempFCB->entry = openedDir.back()->entries[i];
			//assign first cluster
			tempFCB->firstCluster = rootClusters[i];

			//clusterList.push_back(tempFCB->firstCluster);
			uint16_t ptr = tempFCB->firstCluster;
			while(*(FAT[ptr]) != 65535)
			{
				tempFCB->clusterList.push_back(ptr);
				ptr = *(FAT[ptr]);
			}
			tempFCB->size = tempFCB->entry->DSize;
			tempFCB->clusterList.push_back(ptr);
			openedDir.push_back(tempFCB);

			//cout << "name: " << tempFCB->entry->DShortFileName << endl;
			//cout << "clusterList size: " << tempFCB->clusterList.size() << endl;
			combineData(tempFCB->dd);
			//cout << "here" << endl;
			break;
		}
	}

	if(i == openedDir.back()->entries.size())
	{
		cout << "filename not found" << endl;
		cout << "creating file" << endl;
		if(!create)
			cout << "but flag is not O_CREAT!!" << endl;
		//name
		//strcpy(tempFCB->DShortFileName, filename);
		//find empty cluster to assign value to in entry
		tempFCB->firstCluster = findCluster();
		openedDir.push_back(tempFCB);
	}

	/*
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
	*/
	
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

	threads[currentTID]->index = 0;

	char* tempPtr;
	for(i = 0; i < writeCount; i++)
	{
		//findspace return a pointer in memory pool 1
		//cout << "\nsharedmem pool base:" << static_cast<void*>(poolList[1]->basePtr) << endl;
		tempPtr = findSpace(1, lengthList[i], &indexList[i]);
		threads[currentTID]->index = i;
		threads[currentTID]->lengthList = &lengthList;
		threads[currentTID]->indexList = &indexList;
		memcpy(tempPtr, dataList[i], (size_t)lengthList[i]);

		//cout << "tempPtr " << i<< " :" <<static_cast<void*>(tempPtr) << endl;
		//cout << "tempPtr data is: " << tempPtr << endl;
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
	return VM_STATUS_SUCCESS;
}

TVMStatus VMFileClose(int filedescriptor)
{
	/*
	TMachineFileCallback closeCallback;
	closeCallback = &FileCallback;
	threads[currentTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[currentTID]);

	MachineFileClose(filedescriptor, closeCallback, threads[currentTID]);
	scheduler(6);
	*/
	int index = getIndex3(filedescriptor);
	openedDir.erase(openedDir.begin() + index);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMFileRead(int filedescriptor, void *data, int *length)
{
	if(filedescriptor == 0 || filedescriptor == 1 || filedescriptor == filedescriptorInit)
	{
		//cout << "filedescriptor: "<< filedescriptor << endl;
		int index;
	 	//cout <<"\nreading\n\n" << endl;
	 	char* tempPtr = findSpace(1, *length, &index);
		threads[currentTID]->lengthRead =0;
		TMachineFileCallback readCallback;
		readCallback = &FileCallback;
		threads[currentTID]->state = VM_THREAD_STATE_WAITING;
		sleepQ.push_back(threads[currentTID]);

		MemPool* tempPool = poolList[1];
		MachineFileRead(filedescriptor, tempPtr, *length, readCallback, threads[currentTID]);

		if(filedescriptor == 0)
			while(threads[currentTID]->lengthRead ==0);
		else 
			scheduler(6);

		//checkEntries();
		//cout << "ahaahaha" << endl;
		//checkEntries();

		*length = threads[currentTID]->lengthRead;
		memcpy(data, tempPtr, (size_t)*length);//schedlued back from idle after callback
		//cout << "length: " << *length<< endl;
		threads[currentTID]->lengthRead =0;
		freeSpace1(1, *length, index);
	}
	else
	{
		FCB *tempFCB = new FCB;
		int index = getIndex3(filedescriptor);
		tempFCB = openedDir[index];
		if(*length > 1024)
			cout << "reading exceeds 1024" << endl;
		//finished reading

		//cout << "\n\nbytesAccessed: " << tempFCB->bytesAccessed << endl << endl;
		if(tempFCB->bytesAccessed >= tempFCB->entry->DSize)
			return VM_STATUS_FAILURE;
		if(tempFCB->bytesAccessed + *length >= tempFCB->entry->DSize)
			*length = tempFCB->entry->DSize - tempFCB->bytesAccessed;
		memcpy(data, tempFCB->data + tempFCB->bytesAccessed, *length);
		//char * tempData = (void*)data;
		//tempData[*length] = '\0';
		tempFCB->bytesAccessed += *length;
	}
    return VM_STATUS_SUCCESS;
}

TVMStatus VMFileSeek(int filedescriptor, int offset, int whence, int *newoffset)
{
	TMachineFileCallback seekCallback;
	seekCallback = &FileCallback;
	TVMThreadID thisTID = currentTID;
	threads[currentTID]->offset = 0;
	threads[thisTID]->state = VM_THREAD_STATE_WAITING;
	sleepQ.push_back(threads[thisTID]);

	MachineFileSeek(filedescriptor, offset, whence, seekCallback, threads[currentTID]);
	scheduler(6);
	*newoffset = threads[currentTID]->offset;
	threads[currentTID]->offset = 0;
	return VM_STATUS_SUCCESS;
}


}
