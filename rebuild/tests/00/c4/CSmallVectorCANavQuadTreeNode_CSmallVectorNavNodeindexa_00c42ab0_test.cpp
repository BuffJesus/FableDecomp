#include <cstdio>

struct CSmallVector_NavNode {
    void** m_pData;
};

void* __fastcall CSmallVector_NavNode_index_a(CSmallVector_NavNode* self, void* /*edx*/, unsigned long i)
{
    return self->m_pData[i];
}

int main()
{
    void* slots[4];
    slots[0] = (void*)0x1111;
    slots[1] = (void*)0x2222;
    slots[2] = (void*)0x3333;
    slots[3] = (void*)0x4444;

    CSmallVector_NavNode vec;
    vec.m_pData = slots;

    void* r0 = CSmallVector_NavNode_index_a(&vec, 0, 0);
    void* r2 = CSmallVector_NavNode_index_a(&vec, 0, 2);

    if (r0 == slots[0] && r2 == slots[2]) {
        printf("INDEX_A_OK\n");
    } else {
        printf("INDEX_A_FAIL\n");
    }

    return 0;
}