#include "fable_memory_allocator.h"
#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include "fable_variable_size_allocator.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The native virtual operations not exercised by construction fail fast.
void* CMemoryAllocatorFixedSize::Allocate(unsigned long){abort();return 0;}
void CMemoryAllocatorFixedSize::Deallocate(void*){abort();}
unsigned long CMemoryAllocatorFixedSize::GetAllAllocatedAreas(void**,unsigned long){abort();return 0;}
void* CMemoryAllocatorFixedSize::GetAllocatedArea(long){abort();return 0;}
void CMemoryAllocatorFixedSize::Validate(){abort();}
void* CMemoryAllocatorVariableSize::Allocate(unsigned long){abort();return 0;}
void CMemoryAllocatorVariableSize::Deallocate(void*){abort();}
unsigned long CMemoryAllocatorVariableSize::GetAllAllocatedAreas(void**,unsigned long){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetAllocationSize(void*){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetAllocationSizeIncludingHeader(void*){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetLargestFreeAreaSize(){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetTotalMemoryControlledByAllocator(){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetTotalMemoryAllocated(){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetNoFreeAreas(){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetNoAllocatedAreas(){abort();return 0;}
float CMemoryAllocatorVariableSize::GetFragmentation(){abort();return 0;}
void* CMemoryAllocatorVariableSize::GetAllocatedArea(long){abort();return 0;}
unsigned long CMemoryAllocatorVariableSize::GetHeaderOverhead(){abort();return 0;}
void CMemoryAllocatorVariableSize::Validate(){abort();}
CMemoryAllocatorVariableSize::~CMemoryAllocatorVariableSize(){abort();}
CMemoryAllocatorVariableSize::CMemoryAreaChunk* CMemoryAllocatorVariableSize::NewChunk(long){abort();return 0;}
CMemoryAllocatorVariableSize::CMemoryAreaChunk* CMemoryAllocatorVariableSize::NewChunkDemandPaged(void*,long){abort();return 0;}

class Model : public CMemoryAllocator {
public:
    Model(void* heap):CMemoryAllocator(heap){}
    static CAllocatorInfo* Table(){return AllocatorInfo;}
};
FABLE_STATIC_ASSERT(sizeof(Model)==0x49c);
struct Area {unsigned char* Address;unsigned long Size,Kind;};
static Area areas[64];static unsigned long areaCount,virtualCount;
static unsigned long pool[0x40000/4];static unsigned long poolOffset;
static unsigned long pattern,failedVirtual,expectedEventCount,eventIndex;
static int lockDepth;
static const unsigned char* expectedEvents;
static void event(unsigned long kind,unsigned long value=0)
{
    unsigned long words[4]={kind,value,0,0};
    if(eventIndex>=expectedEventCount || memcmp(words,expectedEvents+eventIndex*16,16))abort();
    ++eventIndex;
}
static void* allocate(unsigned long size,unsigned long kind,bool failure)
{
    if(areaCount>=64 || poolOffset+size>sizeof(pool))abort();
    unsigned char* address=failure?0:reinterpret_cast<unsigned char*>(pool)+poolOffset;
    areas[areaCount].Address=address;areas[areaCount].Size=size;areas[areaCount].Kind=kind;++areaCount;
    if(address){memset(address,kind==1?pattern:0xa5,size);poolOffset+=(size+15)&~15UL;}
    return address;
}
extern "C" void __stdcall Init(_RTL_CRITICAL_SECTION* section){if(section!=&g_CMemoryAllocatorCriticalSection_013CAA70)abort();event(1);}
extern "C" void __stdcall Enter(_RTL_CRITICAL_SECTION* section){if(section!=&g_CMemoryAllocatorCriticalSection_013CAA70)abort();event(2);++lockDepth;}
extern "C" void __stdcall Leave(_RTL_CRITICAL_SECTION* section){if(section!=&g_CMemoryAllocatorCriticalSection_013CAA70)abort();event(3);--lockDepth;}
extern "C" void* __stdcall VM(void* address,unsigned long size,unsigned long type,unsigned long protect)
{if(address||type!=0x3000||protect!=4)abort();event(4,size);return allocate(size,1,virtualCount++==failedVirtual);}
extern "C" void* __cdecl Malloc(unsigned int size){event(5,size);return allocate(size,2,false);}
extern "C" void* __stdcall Open(const char* name,unsigned long access,unsigned long share,void* security,unsigned long creation,unsigned long flags,void* templ)
{if(strcmp(name,"boot.ini")||access!=0x80000000||share!=1||security||creation!=3||flags!=0x08000000||templ)abort();event(6);return reinterpret_cast<void*>(0xffffffff);}
extern "C" unsigned long __stdcall Error(){event(7);return 2;}
extern "C" {
void (__stdcall *IC0)(_RTL_CRITICAL_SECTION*)=Init;
void (__stdcall *IC1)(_RTL_CRITICAL_SECTION*)=Enter;
void (__stdcall *IC2)(_RTL_CRITICAL_SECTION*)=Leave;
void* (__stdcall *IVM)(void*,unsigned long,unsigned long,unsigned long)=VM;
void* (__stdcall *IOpen)(const char*,unsigned long,unsigned long,void*,unsigned long,unsigned long,void*)=Open;
unsigned long (__stdcall *IError)()=Error;
}
static unsigned long token(unsigned long pointer)
{
    for(unsigned long i=0;i<areaCount;++i)if(areas[i].Address && pointer==reinterpret_cast<unsigned long>(areas[i].Address))return 0xf0000000+i;
    return pointer;
}
static unsigned char oracle[0x200000];static const unsigned char* cursor;
static unsigned long word(){unsigned long value;memcpy(&value,cursor,4);cursor+=4;return value;}
static void normalize(unsigned char* bytes,unsigned long start,unsigned long end,unsigned long stride)
{for(unsigned long offset=start;offset<end;offset+=stride)*reinterpret_cast<unsigned long*>(bytes+offset)=token(*reinterpret_cast<unsigned long*>(bytes+offset));}
int main(int argc,char** argv)
{
    if(argc!=2)return 1;FILE* file=fopen(argv[1],"rb");if(!file)return 2;unsigned long length=fread(oracle,1,sizeof(oracle),file);fclose(file);if(length==sizeof(oracle))return 3;cursor=oracle;
    unsigned long cases=word();if(cases!=16)return 4;
    for(unsigned long row=0;row<cases;++row) {
        pattern=word();failedVirtual=word();unsigned long expectedAreas=word();expectedEventCount=word();int expectedLock=word();
        memcpy(Model::Table(),cursor,840);cursor+=840;const unsigned char* expectedOwner=cursor;cursor+=0x49c;const unsigned char* expectedTable=cursor;cursor+=840;expectedEvents=cursor;cursor+=expectedEventCount*16;
        areaCount=0;virtualCount=0;poolOffset=0;eventIndex=0;lockDepth=0;
        unsigned long storage[(0x49c+8)/4];unsigned char* bytes=reinterpret_cast<unsigned char*>(storage);memset(bytes,pattern,sizeof(storage));memset(bytes,0x17,4);memset(bytes+4+0x49c,0x31,4);
        Model* owner=::new(bytes+4)Model(reinterpret_cast<void*>(0x12345678));
        normalize(bytes+4,0x460,0x49c,4);normalize(reinterpret_cast<unsigned char*>(Model::Table()),24,840,28);
        if(memcmp(bytes+4,expectedOwner,0x49c)||memcmp(Model::Table(),expectedTable,840))return 5;
        for(int q=0;q<4;++q)if(bytes[q]!=0x17 || bytes[4+0x49c+q]!=0x31)return 6;
        if(areaCount!=expectedAreas || eventIndex!=expectedEventCount || lockDepth!=expectedLock)return 7;
        for(unsigned long i=0;i<expectedAreas;++i) {
            unsigned long size=word(),kind=word(),present=word();
            if(size!=areas[i].Size || kind!=areas[i].Kind || present!=unsigned(areas[i].Address!=0))return 8;
            if(present){if(kind==1)memset(areas[i].Address,0,4);else normalize(areas[i].Address,0,size,8);if(memcmp(areas[i].Address,cursor,size))return 9;cursor+=size;}
        }
        // No destruction: unrecovered virtual operations must not run here.
    }
    if(cursor!=oracle+length)return 10;puts("MAIN_ALLOCATOR_CTOR_OK cases=16");return 0;
}
