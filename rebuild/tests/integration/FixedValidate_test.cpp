#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
class ValidateAccess:public CMemoryAllocatorFixedSize{public:using CMemoryAllocatorFixedSize::Validate;};
static unsigned char expected[0x8000];static unsigned long args[5],eventIndex;
struct Event{unsigned long args[2];unsigned char before[0x8000];};static Event events[32];
static void read(FILE* f,void* p,unsigned long n){if(fread(p,1,n,f)!=n)exit(2);}
#ifdef FABLE_RECORD_VALIDATE
void CMemoryAllocator::InternalValidateArea(void* area)
{
    if(eventIndex>=args[3])exit(7);Event& e=events[eventIndex];
    if(reinterpret_cast<unsigned long>(this)!=e.args[0]||reinterpret_cast<unsigned long>(area)!=e.args[1]||memcmp(reinterpret_cast<void*>(0x2004000),e.before,sizeof(e.before)))exit(8);
    if(args[0]==2&&eventIndex==0){
        *(static_cast<unsigned char*>(area)-3)=255;
        CMemoryAllocatorFixedSize::CMemoryAreaChunk* chunk=reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryAreaChunk*>(args[2]);
        chunk->PNextChunk=chunk->PNextChunk?chunk->PNextChunk->PNextChunk:0;
        g_FableMemoryAllocator_013CAA90=reinterpret_cast<CMemoryAllocator*>(0x2007900);
    }
    ++eventIndex;
}
#endif
int main(int argc,char** argv)
{
    if(argc!=2)return 2;FILE* f=fopen(argv[1],"rb");if(!f)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    ValidateAccess* owner=reinterpret_cast<ValidateAccess*>(0x2004000);unsigned long cases;read(f,&cases,4);
    for(unsigned long i=0;i<cases;++i){
        read(f,args,sizeof(args));if(args[3]>32)return 4;read(f,owner,sizeof(expected));read(f,expected,sizeof(expected));eventIndex=0;
        for(unsigned long j=0;j<args[3];++j){read(f,events[j].args,sizeof(events[j].args));read(f,events[j].before,sizeof(events[j].before));}
        g_FableMemoryAllocator_013CAA90=reinterpret_cast<CMemoryAllocator*>(0x2007800);
        if(args[1])owner->Validate(reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryAreaChunk*>(args[2]));
        else owner->CMemoryAllocatorFixedSize::Validate();
        if(eventIndex!=args[3]||memcmp(owner,expected,sizeof(expected))||reinterpret_cast<unsigned long>(g_FableMemoryAllocator_013CAA90)!=args[4]){printf("fixed validation case %lu failed\n",i);return 5;}
    }
    if(fgetc(f)!=EOF)return 6;printf("FIXED_VALIDATE_OK cases=%lu\n",cases);return 0;
}
