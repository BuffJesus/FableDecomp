#include <stdio.h>

typedef unsigned int size_t_;

struct CFileInfo { int dummy; };

static void* g_lastAllocPtr = 0;
static unsigned int g_lastAllocSize = 0;
static int g_allocFail = 0;
static char g_allocBuf[64];

extern "C" void* __cdecl operator_new_impl(unsigned int size)
{
    g_lastAllocSize = size;
    if (g_allocFail)
    {
        g_lastAllocPtr = 0;
        return 0;
    }
    g_lastAllocPtr = (void*)g_allocBuf;
    return g_lastAllocPtr;
}

struct CCountedBlock
{
    int refCount;
    void* vtable;
    CFileInfo* ptr;
};

struct CCountedPointer_CFileInfo
{
    CFileInfo* m_ptr;
    CCountedBlock* m_block;
};

extern "C" CCountedPointer_CFileInfo* __fastcall CCountedPointer_CFileInfo_construct_b(CCountedPointer_CFileInfo* self, void* edx, CFileInfo* p);

int main()
{
    CFileInfo fi;

    CCountedPointer_CFileInfo obj1;
    obj1.m_ptr = (CFileInfo*)0x12345678;
    obj1.m_block = (CCountedBlock*)0x11111111;
    g_allocFail = 0;
    CCountedPointer_CFileInfo* r1 = CCountedPointer_CFileInfo_construct_b(&obj1, 0, &fi);
    if (r1 != &obj1) { return 1; }
    if (obj1.m_ptr != &fi) { return 2; }
    if (obj1.m_block == 0) { return 3; }
    if (obj1.m_block->refCount != 1) { return 4; }
    if (obj1.m_block->vtable != (void*)0x009a9030) { return 5; }
    if (obj1.m_block->ptr != &fi) { return 6; }
    if (g_lastAllocSize != 0xc) { return 7; }

    CCountedPointer_CFileInfo obj2;
    obj2.m_ptr = (CFileInfo*)0x22222222;
    obj2.m_block = (CCountedBlock*)0x33333333;
    g_allocFail = 1;
    CCountedPointer_CFileInfo* r2 = CCountedPointer_CFileInfo_construct_b(&obj2, 0, &fi);
    if (r2 != &obj2) { return 8; }
    if (obj2.m_ptr != &fi) { return 9; }
    if (obj2.m_block != 0) { return 10; }

    CCountedPointer_CFileInfo obj3;
    obj3.m_ptr = (CFileInfo*)0x44444444;
    obj3.m_block = (CCountedBlock*)0x55555555;
    g_allocFail = 0;
    CCountedPointer_CFileInfo* r3 = CCountedPointer_CFileInfo_construct_b(&obj3, 0, 0);
    if (r3 != &obj3) { return 11; }
    if (obj3.m_ptr != 0) { return 12; }
    if (obj3.m_block != 0) { return 13; }

    printf("PASS_009a9b60_OK\n");
    return 0;
}

extern "C" CCountedPointer_CFileInfo* __fastcall CCountedPointer_CFileInfo_construct_b(CCountedPointer_CFileInfo* self, void* edx, CFileInfo* p)
{
    self->m_ptr = p;
    if (p != 0)
    {
        CCountedBlock* block = (CCountedBlock*)operator_new_impl(0xc);
        if (block != 0)
        {
            CFileInfo* cur = self->m_ptr;
            block->refCount = 1;
            block->vtable = (void*)0x009a9030;
            block->ptr = cur;
            self->m_block = block;
            return self;
        }
        self->m_block = (CCountedBlock*)0;
        return self;
    }
    self->m_block = 0;
    return self;
}
