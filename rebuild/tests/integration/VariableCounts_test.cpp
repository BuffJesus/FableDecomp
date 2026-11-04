#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char before[0x8000];
static void read(FILE* f,void* p,unsigned long n){if(fread(p,1,n,f)!=n)exit(2);}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;FILE* f=fopen(argv[1],"rb");if(!f)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    CMemoryAllocatorVariableSize* owner=reinterpret_cast<CMemoryAllocatorVariableSize*>(0x2004000);
    unsigned long count;read(f,&count,4);
    for(unsigned long i=0;i<count;++i){
        unsigned long args[4];read(f,args,sizeof(args));read(f,before,sizeof(before));memcpy(owner,before,sizeof(before));
        g_FableVariableAllocatorAlignment_013D28D4=128;g_FableVariableAllocatorAlignmentShift_013D28D8=7;
        unsigned long result=args[0]?owner->CMemoryAllocatorVariableSize::GetNoAllocatedAreas():owner->CMemoryAllocatorVariableSize::GetNoFreeAreas();
        if(result!=args[1]||memcmp(owner,before,sizeof(before))||g_FableVariableAllocatorAlignment_013D28D4!=args[2]||g_FableVariableAllocatorAlignmentShift_013D28D8!=args[3])return 5;
    }
    if(fgetc(f)!=EOF)return 6;printf("VARIABLE_COUNTS_OK cases=%lu\n",count);return 0;
}
