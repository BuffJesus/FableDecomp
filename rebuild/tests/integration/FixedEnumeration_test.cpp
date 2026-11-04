#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char before[0x8000],expected[0x8000],normalized[0x8000];
static unsigned long args[6],calls,countResult;
static unsigned char* const region=reinterpret_cast<unsigned char*>(0x2004000);
static bool stateMatches(const unsigned char* state);
static unsigned long __fastcall countThunk(CMemoryAllocatorFixedSize* owner,void*)
{
    if(owner!=reinterpret_cast<CMemoryAllocatorFixedSize*>(region)||!stateMatches(before))exit(7);++calls;
    if(args[1]==0)countResult=owner->CMemoryAllocatorFixedSize::GetNoAllocatedAreas();
    else{
        if(args[1]==2){owner->PFirstChunk=owner->PFirstChunk?owner->PFirstChunk->PNextChunk:0;owner->StatAreaSize=64;}
        countResult=0xffffffff;
    }
    return countResult;
}
static bool stateMatches(const unsigned char* state)
{
    if(*reinterpret_cast<unsigned long*>(0x200b024)!=reinterpret_cast<unsigned long>(&countThunk))return false;
    memcpy(normalized,region,sizeof(normalized));*reinterpret_cast<unsigned long*>(normalized+0x7024)=*reinterpret_cast<const unsigned long*>(state+0x7024);
    return memcmp(normalized,state,sizeof(normalized))==0;
}
static void read(FILE* f,void* p,unsigned long n){if(fread(p,1,n,f)!=n)exit(2);}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;FILE* f=fopen(argv[1],"rb");if(!f)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    CMemoryAllocatorFixedSize* owner=reinterpret_cast<CMemoryAllocatorFixedSize*>(region);unsigned long cases;read(f,&cases,4);
    for(unsigned long i=0;i<cases;++i){
        read(f,args,sizeof(args));read(f,before,sizeof(before));read(f,expected,sizeof(expected));memcpy(region,before,sizeof(before));calls=0;countResult=0;
        if(*reinterpret_cast<unsigned long*>(region)!=0x200b000||*reinterpret_cast<unsigned long*>(0x200b024)!=(args[1]==0?0xa65e50:0x200d000))return 4;
        *reinterpret_cast<unsigned long*>(0x200b024)=reinterpret_cast<unsigned long>(&countThunk);
        unsigned long result=args[0]?reinterpret_cast<unsigned long>(owner->CMemoryAllocatorFixedSize::GetAllocatedArea(static_cast<long>(args[2]))):owner->CMemoryAllocatorFixedSize::GetAllAllocatedAreas(reinterpret_cast<void**>(0x200b800),args[2]);
        if(result!=args[3]||calls!=args[5]||(calls&&countResult!=args[4])||!stateMatches(expected)){printf("fixed enumeration case %lu failed\n",i);return 5;}
    }
    if(fgetc(f)!=EOF)return 6;printf("FIXED_ENUMERATION_OK cases=%lu\n",cases);return 0;
}
