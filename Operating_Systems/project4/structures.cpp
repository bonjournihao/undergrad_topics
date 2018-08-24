struct FCB
{	char name[VM_FILE_SYSTEM_SFN_SIZE];
	SVMDirectoryEntry entry;
	vector<SVMDirectoryEntry*> entries;
	int firstCluster;
	int dd; 
	uint8_t *posPtr; //for file
	int entryCounter; //for directory
	bool isFile; 
	int size; // in cluster
};

vector<uint32_t*> FAT;
FCB *currentDir;
FCB *lastDir;
vector<FCB*> openedDir;
vector<SVMDirectoryEntry*> rootEntries;

	FirstSectorofClusterN = ((N – 2) * bpb->BPB_SecPerClus) + bpb->FirstDataSector;
	
	bpb->FirstRootSector = bpb->BPB_RsvdSecCnt + bpb->BPB_NumFATs * bpb->BPB_FATSz16;
	//at 35
	bpb->RootDirectorySectors = (bpb->BPB_RootEntCnt * 32) / 512;
	//32 sectors in bpb
	bpb->FirstDataSector = bpb->FirstRootSector + bpb->RootDirectorySectors;
	//at 67
	bpb->ClusterCount = (bpb->BPB_TotSec32 - bpb->FirstDataSector) / bpb->BPB_SecPerClus;
	//# clusters = 4096

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
	int ClusterCount;};

typedef struct{
    char DLongFileName[VM_FILE_SYSTEM_MAX_PATH];
    char DShortFileName[VM_FILE_SYSTEM_SFN_SIZE];
    unsigned int DSize;
    unsigned char DAttributes;
    SVMDateTime DCreate;
    SVMDateTime DAccess;
    SVMDateTime DModify;
} SVMDirectoryEntry, *SVMDirectoryEntryRef;