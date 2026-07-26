#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile
{
    void* field_0;
};

static int g_called = 0;
static void* g_arg = 0;

extern "C" void __cdecl sub_79bd72(void* p)
{
    g_called++;
    g_arg = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->field_0;
    if (p != 0)
        sub_79bd72(p);
}

int main()
{
    CActiveFile a;
    int dummy = 42;
    a.field_0 = &dummy;
    CActiveFile_OnReadFinished(&a);

    CActiveFile b;
    b.field_0 = 0;
    CActiveFile_OnReadFinished(&b);

    if (g_called == 1 && g_arg == &dummy)
        printf("OK_00462ca2\n");
    else
        printf("FAIL_00462ca2\n");
    return 0;
}