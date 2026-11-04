#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
typedef CMemoryAllocatorVariableSize::CMemoryArea CMemoryArea;
class TreeAccess : public CMemoryAllocatorVariableSize {
public:
    using CMemoryAllocatorVariableSize::InsertIntoTree;
    using CMemoryAllocatorVariableSize::InsertFixup;
    using CMemoryAllocatorVariableSize::RotateLeft;
    using CMemoryAllocatorVariableSize::RotateRight;
};
static unsigned char expected[0x8000];
static void read(FILE* input,void* data,size_t size)
{
    if(fread(data,1,size,input)!=size){puts("short oracle");exit(2);}
}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;
    void* storage=VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4);
    if(storage!=reinterpret_cast<void*>(0x2000000)){puts("fixed test address unavailable");return 3;}
    FILE* input=fopen(argv[1],"rb");if(!input)return 2;
    unsigned char* region=reinterpret_cast<unsigned char*>(0x2004000);
    TreeAccess* owner=reinterpret_cast<TreeAccess*>(region);
    unsigned long count,snapshots=0;read(input,&count,4);
    for(unsigned long sequenceIndex=0;sequenceIndex<count;++sequenceIndex) {
        unsigned long pattern,length,pairs[64][2];read(input,&pattern,4);read(input,&length,4);
        if(length>64)return 2;read(input,pairs,length*8);memset(region,pattern,0x8000);owner->PTreeRoot=0;
        for(unsigned long i=0;i<length;++i) {
            CMemoryArea* area=reinterpret_cast<CMemoryArea*>(0x2008000+pairs[i][0]*0x40);
            area->PRightChild=0;area->PLeftChild=0;area->PParentArea=0;area->DataSize=pairs[i][1];
        }
        for(i=0;i<length;++i) {
            CMemoryArea* area=reinterpret_cast<CMemoryArea*>(0x2008000+pairs[i][0]*0x40);
            owner->InsertIntoTree(owner->PTreeRoot,area);read(input,expected,sizeof(expected));++snapshots;
            if(memcmp(region,expected,sizeof(expected))){printf("sequence %lu insert %lu mismatch\n",sequenceIndex,i);return 4;}
        }
    }
    unsigned long operations;read(input,&operations,4);
    for(unsigned long operationIndex=0;operationIndex<operations;++operationIndex) {
        unsigned long mode,address;read(input,&mode,4);read(input,&address,4);
        read(input,region,0x8000);read(input,expected,sizeof(expected));
        CMemoryArea* area=reinterpret_cast<CMemoryArea*>(address);
        if(mode==1)owner->RotateLeft(area);else if(mode==2)owner->RotateRight(area);else owner->InsertFixup(area);
        if(memcmp(region,expected,sizeof(expected))){printf("operation %lu mismatch\n",operationIndex);return 5;}
    }
    if(fgetc(input)!=EOF)return 6;fclose(input);VirtualFree(storage,0,0x8000);
    printf("VARIABLE_TREE_INSERTION_OK sequences=%lu snapshots=%lu operations=%lu\n",count,snapshots,operations);return 0;
}
