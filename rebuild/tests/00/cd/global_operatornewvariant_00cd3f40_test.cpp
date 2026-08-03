#include <stdio.h>

extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*, unsigned long, unsigned long, unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*, unsigned long, unsigned long);

static const unsigned long kMemCommit = 0x1000;
static const unsigned long kMemReserve = 0x2000;
static const unsigned long kMemRelease = 0x8000;
static const unsigned long kPageReadWrite = 0x04;

class CFixedAllocator
{
public:
    void* Allocate(unsigned int size)
    {
        lastSize = size;
        return (void*)0x12345678;
    }
    unsigned int lastSize;
};

#define g_FixedAllocator_0143e920 (*(CFixedAllocator*)0x0143e920)

void* __fastcall operator_new_variant(unsigned int size)
{
    return g_FixedAllocator_0143e920.Allocate(size);
}

int main()
{
    void* page = VirtualAlloc(reinterpret_cast<void*>(0x0143e000), 0x1000,
        kMemReserve | kMemCommit, kPageReadWrite);
    if (page == 0) { printf("FAIL\n"); return 1; }
    CFixedAllocator* allocator = reinterpret_cast<CFixedAllocator*>(0x0143e920);
    allocator->lastSize = 0;

    void* p = operator_new_variant(0x40);

    int ok = (p == (void*)0x12345678 && allocator->lastSize == 0x40);
    VirtualFree(page, 0, kMemRelease);

    if (ok)
    {
        const char* marker = "PASS_OPERATOR_NEW_VARIANT";
        printf("%s\n", marker);
        return 0;
    }
    printf("FAIL\n");
    return 1;
}