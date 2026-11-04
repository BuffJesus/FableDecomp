#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char before[0x8000];
static unsigned long event[3],calls,returned;
CMemoryAllocatorVariableSize::CMemoryArea* CMemoryAllocatorVariableSize::CMemoryAreaChunk::AllocAreaFromArea(CMemoryArea* area,long size)
{
    ++calls;event[0]=reinterpret_cast<unsigned long>(this);event[1]=reinterpret_cast<unsigned long>(area);event[2]=size;
    return reinterpret_cast<CMemoryArea*>(returned);
}
static void read(FILE* f,void* p,unsigned long n){if(fread(p,1,n,f)!=n)exit(2);}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;FILE* f=fopen(argv[1],"rb");if(!f)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    unsigned char* region=reinterpret_cast<unsigned char*>(0x2004000);
    CMemoryAllocatorVariableSize* owner=reinterpret_cast<CMemoryAllocatorVariableSize*>(region);
    unsigned long count;read(f,&count,4);
    for(unsigned long i=0;i<count;++i){
        unsigned long args[7];read(f,args,sizeof(args));read(f,region,0x8000);memcpy(before,region,sizeof(before));
        returned=args[1];calls=0;memset(event,0,sizeof(event));
        unsigned long result=reinterpret_cast<unsigned long>(owner->AllocateMemoryFromExistingChunks(args[0]));
        if(result!=args[2]||calls!=args[3]||memcmp(event,args+4,sizeof(event))||memcmp(region,before,sizeof(before))){printf("case %lu mismatch\n",i);return 5;}
    }
    if(fgetc(f)!=EOF)return 6;
    printf("VARIABLE_ALLOCATE_EXISTING_OK cases=%lu\n",count);return 0;
}
