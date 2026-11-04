#include "fable_memory_allocator.h"
#include <new>
#include <stdio.h>
#include <string.h>
class Model : public CMemoryAllocatorBase {
public:
    Model(unsigned long alignment):CMemoryAllocatorBase(alignment){}
    void* Allocate(unsigned long){return 0;} void Deallocate(void*){}
    unsigned long GetAllAllocatedAreas(void**,unsigned long){return 0;}
    unsigned long GetAllocationSize(void*){return 0;}
    unsigned long GetAllocationSizeIncludingHeader(void*){return 0;}
    unsigned long GetLargestFreeAreaSize(){return 0;}
    unsigned long GetTotalMemoryControlledByAllocator(){return 0;}
    unsigned long GetTotalMemoryAllocated(){return 0;}
    unsigned long GetNoFreeAreas(){return 0;}
    unsigned long GetNoAllocatedAreas(){return 0;}
    float GetFragmentation(){return 0;}
    void* GetAllocatedArea(long){return 0;}
    unsigned long GetHeaderOverhead(){return 0;}
    void Validate(){}
};
// RETAIL_EXPECTATIONS
int main()
{
    static const unsigned long alignments[10]={0,1,2,3,4,5,8,16,32,0xffffffff};
    unsigned long storage[(0xc1c+8)/4];unsigned char* bytes=reinterpret_cast<unsigned char*>(storage);
    for(int i=0;i<20;++i) {
        memset(bytes,i%2?0xa5:0x5a,sizeof(storage));memset(bytes,0x17,4);memset(bytes+4+0xc1c,0x31,4);
        Model* model=::new(bytes+4)Model(alignments[i/2]);
        // Derived construction installs its own vptr; all base data must match.
        if(memcmp(bytes+8,expected[i]+4,0xc18))return 1;
        for(int q=0;q<4;++q)if(bytes[q]!=0x17 || bytes[4+0xc1c+q]!=0x31)return 2;
        model->~Model();
    }
    puts("ALLOCATOR_BASE_CTOR_OK cases=20");return 0;
}
