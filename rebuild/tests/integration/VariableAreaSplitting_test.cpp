#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
class ChunkAccess:public CMemoryAllocatorVariableSize::CMemoryAreaChunk {
public:
    void RemoveTest(CMemoryAllocatorVariableSize::CMemoryArea* p){RemoveAreaFromFreeList(p);}
    void InsertTest(CMemoryAllocatorVariableSize::CMemoryArea* p){InsertAreaIntoAllocatedList(p);}
};
static FILE* input;
static unsigned long calls;
static unsigned char expected[0x8000],snapshot[0x8000];
static unsigned char* const region=reinterpret_cast<unsigned char*>(0x2004000);
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static void boundary(unsigned long address,unsigned long owner,unsigned long a,unsigned long b)
{
    unsigned long event[4];read(event,sizeof(event));read(snapshot,sizeof(snapshot));
    if(event[0]!=address||event[1]!=owner||event[2]!=a||event[3]!=b||memcmp(region,snapshot,sizeof(snapshot)))exit(7);
    ++calls;
}
void CMemoryAllocatorVariableSize::RemoveFromTree(CMemoryArea* p)
{boundary(0xa64ef0,reinterpret_cast<unsigned long>(this),reinterpret_cast<unsigned long>(p),0);}
void CMemoryAllocatorVariableSize::InsertIntoTree(CMemoryArea* root,CMemoryArea* p)
{boundary(0xa64e70,reinterpret_cast<unsigned long>(this),reinterpret_cast<unsigned long>(root),reinterpret_cast<unsigned long>(p));}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    unsigned long count;read(&count,4);
    for(unsigned long i=0;i<count;++i){
        unsigned long args[5];read(args,sizeof(args));read(region,0x8000);read(expected,sizeof(expected));calls=0;
        g_FableVariableAllocatorAlignment_013D28D4=args[1];
        g_FableVariableAllocatorAlignmentShift_013D28D8=args[1]==4?2:args[1]==8?3:args[1]==16?4:5;
        ChunkAccess* chunk=reinterpret_cast<ChunkAccess*>(0x2009000);
        CMemoryAllocatorVariableSize::CMemoryArea* area=reinterpret_cast<CMemoryAllocatorVariableSize::CMemoryArea*>(0x200a000);
        unsigned long result=0;
        if(args[0]==0)result=reinterpret_cast<unsigned long>(chunk->AllocAreaFromArea(area,args[2]));
        else if(args[0]==1)chunk->RemoveTest(area);
        else chunk->InsertTest(area);
        if(result!=args[3]||calls!=args[4]||memcmp(region,expected,sizeof(expected))){printf("case %lu mismatch\n",i);return 5;}
    }
    if(fgetc(input)!=EOF)return 6;
    printf("VARIABLE_AREA_SPLITTING_OK cases=%lu\n",count);return 0;
}
