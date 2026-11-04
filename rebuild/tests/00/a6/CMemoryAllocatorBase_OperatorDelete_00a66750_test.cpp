#include "fable_memory_allocator.h"
#include <stdio.h>
#if defined(FABLE_CATALOG_NEW_PROVIDER)
#include "../../../src/compiled/00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750.cpp"
#elif defined(FABLE_CATALOG_DELETE_PROVIDER)
#include "../../../src/compiled/00/a6/CMemoryAllocatorBase_OperatorNew_00a66730.cpp"
#endif
struct MemoryInformation {
    void* BaseAddress;void* AllocationBase;unsigned long AllocationProtect;
    unsigned long RegionSize;unsigned long State;unsigned long Protect;unsigned long Type;
};
extern "C" __declspec(dllimport) unsigned long __stdcall VirtualQuery(const void*,MemoryInformation*,unsigned long);
int main()
{
    static const unsigned long sizes[3]={4096,8192,1048576};
    for(int i=0;i<3;++i) {
        void* area=CMemoryAllocatorBase::operator new(sizes[i]);
        if(!area)return 1;
        unsigned char* bytes=static_cast<unsigned char*>(area);
        if(bytes[0] || bytes[sizes[i]-1])return 3;
        bytes[0]=0x13;bytes[sizes[i]-1]=0x57;
        if(bytes[0]!=0x13 || bytes[sizes[i]-1]!=0x57)return 4;
        CMemoryAllocatorBase::operator delete(area);
        MemoryInformation information;
        if(VirtualQuery(area,&information,sizeof(information))!=sizeof(information) || information.State!=0x10000)return 2;
    }
    CMemoryAllocatorBase::operator delete(0); // retail forwards null and ignores failure
    puts("ALLOCATOR_BASE_DELETE_OK cases=4");return 0;
}
