#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile
{
    void* m_ptr;
};

static int g_called = 0;
static void* g_arg = 0;

extern void __cdecl CActiveFile_ReleaseCallee(void* p);
void __cdecl CActiveFile_ReleaseCallee(void* p) { g_called++; g_arg = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

int main()
{
    int dummy = 0x1234;

    /* non-null: callee invoked with the pointer */
    CActiveFile a;
    a.m_ptr = &dummy;
    g_called = 0; g_arg = 0;
    CActiveFile_OnReadFinished(&a);

    /* null: callee NOT invoked */
    CActiveFile b;
    b.m_ptr = 0;
    CActiveFile_OnReadFinished(&b);

    if (g_called == 1 && g_arg == &dummy)
        printf("OK_00462595\n");
    else
        printf("FAIL_00462595\n");
    return 0;
}