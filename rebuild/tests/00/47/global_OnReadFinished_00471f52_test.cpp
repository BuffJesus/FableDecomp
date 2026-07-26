#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile
{
    void* ptr;
};

static int g_calls = 0;
static void* g_last = 0;

extern "C" void __cdecl sub_78cac2(void* p)
{
    g_calls++;
    g_last = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

int main()
{
    /* non-null: callee invoked */
    int dummy = 0;
    CActiveFile a;
    a.ptr = &dummy;
    CActiveFile_OnReadFinished(&a);

    /* null: callee skipped */
    CActiveFile b;
    b.ptr = 0;
    CActiveFile_OnReadFinished(&b);

    if (g_calls == 1 && g_last == &dummy)
    {
        printf("PASS_00471f52\n");
    }
    else
    {
        printf("FAIL_00471f52 calls=%d\n", g_calls);
    }
    return 0;
}