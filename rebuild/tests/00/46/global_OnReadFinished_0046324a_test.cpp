#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile_46324a {
    void* m_ptr;
};

extern void __fastcall OnReadFinished_46324a(CActiveFile_46324a* thisptr);

static int g_freed = 0;
static void* g_lastFreed = 0;

extern "C" void __cdecl freeBuffer_46324a(void* p)
{
    g_freed++;
    g_lastFreed = p;
}

int main()
{
    int dummy = 0;
    CActiveFile_46324a a;
    a.m_ptr = &dummy;
    OnReadFinished_46324a(&a);

    CActiveFile_46324a b;
    b.m_ptr = 0;
    OnReadFinished_46324a(&b);

    if (g_freed == 1 && g_lastFreed == (void*)&dummy) {
        printf("PASS_46324a_OK\n");
    } else {
        printf("FAIL_46324a\n");
    }
    return 0;
}