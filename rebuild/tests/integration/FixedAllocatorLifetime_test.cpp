#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Narrow constructor/destructor fixture boundaries, never called here.
// gate_fixed_os_lifetime.py links and exercises the recovered native providers.
void* CMemoryAllocatorFixedSize::Allocate(unsigned long){abort();return 0;}
void CMemoryAllocatorFixedSize::Deallocate(void*){abort();}
unsigned long CMemoryAllocatorFixedSize::GetAllAllocatedAreas(void**,unsigned long){abort();return 0;}
void* CMemoryAllocatorFixedSize::GetAllocatedArea(long){abort();return 0;}
void CMemoryAllocatorFixedSize::Validate(){abort();}
// RETAIL_EXPECTATIONS
int main()
{
    unsigned long storage[(0xc4c+8)/4];unsigned char* bytes=reinterpret_cast<unsigned char*>(storage);
    for(int i=0;i<30;++i) {
        memset(bytes,i%2?0xa5:0x5a,sizeof(storage));memset(bytes,0x17,4);memset(bytes+4+0xc4c,0x31,4);
        CMemoryAllocatorFixedSize* allocator=::new(bytes+4)CMemoryAllocatorFixedSize(
            reinterpret_cast<void*>(0x12345678),arguments[i/2][0],arguments[i/2][1],arguments[i/2][2],arguments[i/2][3],arguments[i/2][4]);
        unsigned long vptr=*reinterpret_cast<unsigned long*>(allocator);
        if(memcmp(bytes+8,expected[i]+4,0xc48))return 1;
        for(int q=0;q<4;++q)if(bytes[q]!=0x17 || bytes[4+0xc4c+q]!=0x31)return 2;
        *reinterpret_cast<unsigned long*>(allocator)=0xdeadbeef;
        allocator->CMemoryAllocatorFixedSize::~CMemoryAllocatorFixedSize();
        if(*reinterpret_cast<unsigned long*>(allocator)!=vptr || memcmp(bytes+8,expected[i]+4,0xc48))return 3;
        for(int q=0;q<4;++q)if(bytes[q]!=0x17 || bytes[4+0xc4c+q]!=0x31)return 4;
    }
    puts("FIXED_ALLOCATOR_LIFETIME_OK cases=30");return 0;
}
