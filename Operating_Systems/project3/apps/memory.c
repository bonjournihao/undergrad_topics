#include "VirtualMachine.h"

void VMMain(int argc, char *argv[]){
    int Index, Inner;
    int *Pointers[4];
    int *MemoryBase;
    TVMMemoryPoolID MemoryPoolID;
    TVMMemorySize MemorySize;
    
    //allocating space from system pool
    VMPrint("Allocating pool: ");
    if(VM_STATUS_SUCCESS != VMMemoryPoolAllocate(VM_MEMORY_POOL_ID_SYSTEM, 256, (void **)&MemoryBase)){
        VMPrintError("Failed to allocate memory pool\n");
        return;
    }
    //create a pool with 256 bytes
    VMPrint("Done\nCreating pool: ");
    if(VM_STATUS_SUCCESS != VMMemoryPoolCreate(MemoryBase, 256, &MemoryPoolID)){
        VMMemoryPoolDeallocate(VM_MEMORY_POOL_ID_SYSTEM, MemoryBase);
        VMPrintError("Failed to create memory pool\n");
        return;
    }       
    //allocate 4 chunks of memories each 64 bytes, in pointers[index] 
    VMPrint("Done\nAllocating spaces: ");
    for(Index = 0; Index < 4; Index++){
        if(VM_STATUS_SUCCESS != VMMemoryPoolAllocate(MemoryPoolID, 64, (void **)&Pointers[Index])){
            VMPrintError("Failed to allocate space %d\n", Index);
            return;
        }
        if(VM_STATUS_SUCCESS != VMMemoryPoolQuery(MemoryPoolID, &MemorySize)){
            VMPrintError("Failed to query memory pool\n");
            return;
        }
        VMPrint("%d ", MemorySize);
    }
    //assign values for each int pointer (each has size 64 bytes; 16 nums in each)
    VMPrint("Done\nAssigning values: ");    
    for(Index = 0; Index < 4; Index++){
        for(Inner = 0; Inner < 64 / sizeof(int); Inner++){
            Pointers[Index][Inner] = Index * (64 / sizeof(int)) + Inner;   
        }
    }
    //deallocate space into the previous 256 byte memory pool
    VMPrint("Done\nDeallocating spaces: ");   
    for(Index = 0; Index < 4; Index++){
        if(VM_STATUS_SUCCESS != VMMemoryPoolDeallocate(MemoryPoolID, Pointers[Index])){
            VMPrintError("Failed to deallocate space %d\n", Index);
            return;
        }
    }
    //allocate a memory of 256 bytes
    VMPrint("Done\nAllocating full space: ");   
    if(VM_STATUS_SUCCESS != VMMemoryPoolAllocate(MemoryPoolID, 256, (void **)&Pointers[0])){
        VMPrintError("Failed to allocate full space\n");
        return;
    }
    //printing the values that has been in the same memory
    VMPrint("Done\nPrinting values:");
    for(Index = 0; Index < (256 / sizeof(int)); Index++){
        VMPrint(" %d", Pointers[0][Index]);
    }
    //deallocate the 256 bytes
    VMPrint("\nDeallocating space: "); 
    if(VM_STATUS_SUCCESS != VMMemoryPoolDeallocate(MemoryPoolID, Pointers[0])){
        VMPrintError("Failed to deallocate full space\n");
        return;
    }
    //simply delete a memory pool
    VMPrint("Done\nDeleting memory pool: "); 
    if(VM_STATUS_SUCCESS != VMMemoryPoolDelete(MemoryPoolID)){
        VMPrintError("Failed to delete memory pool\n");
        return;
    }
    //deallocate system memory pool space
    VMPrint("Done\nDeallocating memory pool space: "); 
    if(VM_STATUS_SUCCESS != VMMemoryPoolDeallocate(VM_MEMORY_POOL_ID_SYSTEM, MemoryBase)){
        VMPrintError("Failed to deallocate memory pool space\n");
        return;
    }
    VMPrint("Done\n");
}

