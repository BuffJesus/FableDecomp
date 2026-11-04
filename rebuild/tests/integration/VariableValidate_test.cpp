#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char expected[0x8000];
static unsigned long mode,eventCount,eventIndex;
struct Event{unsigned long args[5];unsigned char before[0x8000];};
static Event events[32];
static void read(FILE* f,void* p,unsigned long n){if(fread(p,1,n,f)!=n)exit(2);}
#ifdef FABLE_RECORD_VALIDATE
void CMemoryAllocator::InternalValidateArea(void* area)
{
    if(eventIndex>=eventCount)exit(7);Event& e=events[eventIndex];
    if(reinterpret_cast<unsigned long>(this)!=e.args[0]||reinterpret_cast<unsigned long>(area)!=e.args[1]||g_FableVariableAllocatorAlignment_013D28D4!=e.args[2]||g_FableVariableAllocatorAlignmentShift_013D28D8!=e.args[3]||reinterpret_cast<unsigned long>(g_FableMemoryAllocator_013CAA90)!=e.args[4]||memcmp(reinterpret_cast<void*>(0x2004000),e.before,sizeof(e.before)))exit(8);
    if(mode==2&&eventIndex==0){
        *reinterpret_cast<unsigned long*>(0x2004c14)=128;*reinterpret_cast<unsigned long*>(0x2004c18)=7;
        g_FableVariableAllocatorAlignment_013D28D4=64;g_FableVariableAllocatorAlignmentShift_013D28D8=6;
        g_FableMemoryAllocator_013CAA90=reinterpret_cast<CMemoryAllocator*>(0x2007900);
    }
    ++eventIndex;
}
#endif
int main(int argc,char** argv)
{
    if(argc!=2)return 2;FILE* f=fopen(argv[1],"rb");if(!f)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    CMemoryAllocatorVariableSize* owner=reinterpret_cast<CMemoryAllocatorVariableSize*>(0x2004000);
    unsigned long cases;read(f,&cases,4);
    for(unsigned long i=0;i<cases;++i){
        unsigned long args[5];read(f,args,sizeof(args));mode=args[0];eventCount=args[1];eventIndex=0;
        if(eventCount>32)return 4;read(f,owner,sizeof(expected));read(f,expected,sizeof(expected));
        for(unsigned long j=0;j<eventCount;++j){read(f,events[j].args,sizeof(events[j].args));read(f,events[j].before,sizeof(events[j].before));}
        g_FableVariableAllocatorAlignment_013D28D4=256;g_FableVariableAllocatorAlignmentShift_013D28D8=8;
        g_FableMemoryAllocator_013CAA90=reinterpret_cast<CMemoryAllocator*>(0x2007800);
        owner->CMemoryAllocatorVariableSize::Validate();
        if(eventIndex!=eventCount||memcmp(owner,expected,sizeof(expected))||g_FableVariableAllocatorAlignment_013D28D4!=args[2]||g_FableVariableAllocatorAlignmentShift_013D28D8!=args[3]||reinterpret_cast<unsigned long>(g_FableMemoryAllocator_013CAA90)!=args[4]){printf("validation case %lu failed\n",i);return 5;}
    }
    if(fgetc(f)!=EOF)return 6;printf("VARIABLE_VALIDATE_OK cases=%lu\n",cases);return 0;
}
