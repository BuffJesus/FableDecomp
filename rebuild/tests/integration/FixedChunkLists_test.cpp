#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
class ListAccess:public CMemoryAllocatorFixedSize{public:using CMemoryAllocatorFixedSize::RemoveChunkFromList;using CMemoryAllocatorFixedSize::InsertChunkAtListHead;using CMemoryAllocatorFixedSize::InsertChunkAtListTail;};
static unsigned char expected[0x8000];
static void read(FILE* f,void* p,unsigned long n){if(fread(p,1,n,f)!=n)exit(2);}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;FILE* f=fopen(argv[1],"rb");if(!f)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    ListAccess* owner=reinterpret_cast<ListAccess*>(0x2004000);unsigned long cases;read(f,&cases,4);
    for(unsigned long i=0;i<cases;++i){
        unsigned long args[2];read(f,args,sizeof(args));read(f,owner,sizeof(expected));read(f,expected,sizeof(expected));
        CMemoryAllocatorFixedSize::CMemoryAreaChunk* chunk=reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryAreaChunk*>(args[1]);
        if(args[0]==0)owner->RemoveChunkFromList(chunk);else if(args[0]==1)owner->InsertChunkAtListHead(chunk);else owner->InsertChunkAtListTail(chunk);
        if(memcmp(owner,expected,sizeof(expected)))return 5;
    }
    if(fgetc(f)!=EOF)return 6;printf("FIXED_CHUNK_LISTS_OK cases=%lu\n",cases);return 0;
}
