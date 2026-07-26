#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile
{
    void* m_ptr;
};

extern "C" void __cdecl CActiveFile_ReleaseBuffer(void* p);
void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

static int g_called = 0;
static void* g_arg = 0;
extern "C" void __cdecl CActiveFile_ReleaseBuffer(void* p)
{
    g_called++;
    g_arg = p;
}

int main()
{
    CActiveFile a;
    int dummy = 42;

    /* null branch: no call */
    a.m_ptr = 0;
    CActiveFile_OnReadFinished(&a);

    /* non-null branch: call with the pointer */
    a.m_ptr = &dummy;
    CActiveFile_OnReadFinished(&a);

    if (g_called == 1 && g_arg == (void*)&dummy)
        printf("PASS_00460d67\n");
    else
        printf("FAIL_00460d67\n");
    return 0;
}