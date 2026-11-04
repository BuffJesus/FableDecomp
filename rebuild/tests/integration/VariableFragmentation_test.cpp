#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char initial[0x8000],after[0x8000],eventStates[2][0x8000],normalized[0x8000];
static unsigned long args[9],events[2][3],eventIndex;
static float zero=0.0f,unsignedScale=4294967296.0f;
static unsigned long __fastcall FreeCount(CMemoryAllocatorVariableSize*,void*);
static unsigned long __fastcall AllocatedCount(CMemoryAllocatorVariableSize*,void*);
static unsigned long __fastcall AlternateCount(CMemoryAllocatorVariableSize*,void*);
static void check(const unsigned char* expected)
{
    memcpy(normalized,reinterpret_cast<void*>(0x2004000),sizeof(normalized));
    unsigned long offsets[4]={0x7020,0x7024,0x7120,0x7124};
    unsigned long native[4]={reinterpret_cast<unsigned long>(FreeCount),reinterpret_cast<unsigned long>(AllocatedCount),reinterpret_cast<unsigned long>(FreeCount),reinterpret_cast<unsigned long>(AlternateCount)};
    unsigned long canonical[4]={0x200d000,0x200d100,0x200d000,0x200d200};
    for(unsigned long i=0;i<4;++i){if(memcmp(normalized+offsets[i],native+i,4))exit(10);memcpy(normalized+offsets[i],canonical+i,4);}
    if(memcmp(normalized,expected,sizeof(normalized)))exit(11);
}
static void event(unsigned long kind,CMemoryAllocatorVariableSize* owner)
{
    if(eventIndex>=args[8]||reinterpret_cast<unsigned long>(owner)!=0x2004000)return exit(12);
    unsigned long got[3]={kind,g_FableVariableAllocatorAlignment_013D28D4,g_FableVariableAllocatorAlignmentShift_013D28D8};
    if(memcmp(got,events[eventIndex],sizeof(got)))exit(13);check(eventStates[eventIndex]);++eventIndex;
}
static unsigned long __fastcall FreeCount(CMemoryAllocatorVariableSize* owner,void*)
{
    event(1,owner);
    if(args[0])return owner->CMemoryAllocatorVariableSize::GetNoFreeAreas();
    if(args[3]){
        *reinterpret_cast<unsigned long*>(owner)=0x200b100;
        unsigned long* fields=reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(owner)+0xc14);fields[0]=64;fields[1]=6;
        g_FableVariableAllocatorAlignment_013D28D4=64;g_FableVariableAllocatorAlignmentShift_013D28D8=6;
    }
    return args[1];
}
static unsigned long __fastcall AllocatedCount(CMemoryAllocatorVariableSize* owner,void*)
{
    event(2,owner);return args[0]?owner->CMemoryAllocatorVariableSize::GetNoAllocatedAreas():args[2];
}
static unsigned long __fastcall AlternateCount(CMemoryAllocatorVariableSize* owner,void*)
{
    event(3,owner);return args[2];
}
static void read(FILE* f,void* p,unsigned long n){if(fread(p,1,n,f)!=n)exit(2);}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;FILE* f=fopen(argv[1],"rb");if(!f)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    // Original retail instructions are a bounded test oracle, never production source.
    unsigned char code[102];read(f,code,sizeof(code));
    unsigned long offsets[5]={11,23,51,74,92};
    unsigned long bindings[5]={reinterpret_cast<unsigned long>(&g_FableVariableAllocatorAlignmentShift_013D28D8),reinterpret_cast<unsigned long>(&g_FableVariableAllocatorAlignment_013D28D4),reinterpret_cast<unsigned long>(&zero),reinterpret_cast<unsigned long>(&unsignedScale),reinterpret_cast<unsigned long>(&unsignedScale)};
    for(unsigned long k=0;k<5;++k)memcpy(code+offsets[k],bindings+k,4);
    void* oracle=VirtualAlloc(0,sizeof(code),0x3000,0x40);if(!oracle)return 4;memcpy(oracle,code,sizeof(code));
    typedef float (__fastcall* Reference)(CMemoryAllocatorVariableSize*,void*);Reference reference=reinterpret_cast<Reference>(oracle);
    CMemoryAllocatorVariableSize* owner=reinterpret_cast<CMemoryAllocatorVariableSize*>(0x2004000);
    unsigned long count;read(f,&count,4);
    for(unsigned long i=0;i<count;++i){
        read(f,args,sizeof(args));if(args[8]!=2)return 5;read(f,initial,sizeof(initial));read(f,after,sizeof(after));
        for(unsigned long j=0;j<2;++j){read(f,events[j],sizeof(events[j]));read(f,eventStates[j],sizeof(eventStates[j]));}
        unsigned long bits[2],status[2];
        for(unsigned long run=0;run<2;++run){
            memcpy(owner,initial,sizeof(initial));
            *reinterpret_cast<unsigned long*>(0x200b020)=reinterpret_cast<unsigned long>(FreeCount);*reinterpret_cast<unsigned long*>(0x200b024)=reinterpret_cast<unsigned long>(AllocatedCount);
            *reinterpret_cast<unsigned long*>(0x200b120)=reinterpret_cast<unsigned long>(FreeCount);*reinterpret_cast<unsigned long*>(0x200b124)=reinterpret_cast<unsigned long>(AlternateCount);
            eventIndex=0;g_FableVariableAllocatorAlignment_013D28D4=256;g_FableVariableAllocatorAlignmentShift_013D28D8=8;
            _controlfp((args[4]?_PC_64:_PC_53)|_RC_NEAR|_MCW_EM,_MCW_PC|_MCW_RC|_MCW_EM);_clearfp();
            float result=run?reference(owner,0):owner->CMemoryAllocatorVariableSize::GetFragmentation();
            memcpy(bits+run,&result,4);status[run]=_statusfp();
            if((_controlfp(0,0)&(_MCW_PC|_MCW_RC))!=(args[4]?_PC_64:_PC_53))return 6;
            check(after);
            if(eventIndex!=2||g_FableVariableAllocatorAlignment_013D28D4!=args[6]||g_FableVariableAllocatorAlignmentShift_013D28D8!=args[7])return 7;
        }
        if(bits[0]!=args[5]||bits[1]!=args[5]||status[0]!=status[1]){printf("case %lu bits %08lx/%08lx expected %08lx status %08lx/%08lx\n",i,bits[0],bits[1],args[5],status[0],status[1]);return 8;}
    }
    if(fgetc(f)!=EOF)return 9;printf("VARIABLE_FRAGMENTATION_OK cases=%lu\n",count);return 0;
}
