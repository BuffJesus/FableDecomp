#include "rebuild_abi.h"
#include <stdio.h>

struct CActiveFile
{
    void* m_ptr;   // +0x00
};

extern "C" void __cdecl sub_78C9DC_free(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

static int g_freed_count = 0;
static void* g_freed_ptr = 0;

extern "C" void __cdecl sub_78C9DC_free(void* p)
{
    g_freed_count++;
    g_freed_ptr = p;
}

int main(void)
{
    /* null pointer: no free call */
    CActiveFile a;
    a.m_ptr = 0;
    CActiveFile_OnReadFinished(&a);

    /* non-null pointer: free called once with that pointer */
    int sentinel = 42;
    CActiveFile b;
    b.m_ptr = &sentinel;
    CActiveFile_OnReadFinished(&b);

    if (g_freed_count == 1 && g_freed_ptr == (void*)&sentinel)
        printf("OK_472038\n");
    else
        printf("FAIL_472038 c=%d\n", g_freed_count);
    return 0;
}