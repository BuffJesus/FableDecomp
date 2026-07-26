#include "rebuild_abi.h"
#include <stdio.h>

struct CActiveFile
{
    void* m_ptr;
};

static int g_called = 0;
static void* g_arg = 0;

extern "C" void sub_7a2d30(void* p)
{
    g_called++;
    g_arg = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

int main()
{
    int dummy = 0x1234;
    CActiveFile a;
    a.m_ptr = &dummy;
    CActiveFile_OnReadFinished(&a);

    CActiveFile b;
    b.m_ptr = 0;
    CActiveFile_OnReadFinished(&b);

    if (g_called == 1 && g_arg == &dummy)
        printf("PASS_0045bce4\n");
    else
        printf("FAIL_0045bce4 called=%d\n", g_called);
    return 0;
}