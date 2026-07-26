#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile
{
    void* m_ptr;
};

extern "C" void __cdecl engine_free_460c33(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

static int g_freed = 0;
extern "C" void __cdecl engine_free_460c33(void* p)
{
    if (p != 0) g_freed = 1;
}

int main()
{
    CActiveFile a;
    a.m_ptr = (void*)0x12345678;
    CActiveFile_OnReadFinished(&a);

    CActiveFile b;
    b.m_ptr = 0;
    g_freed = 0;
    CActiveFile_OnReadFinished(&b);
    int nullcase_ok = (g_freed == 0);

    if (nullcase_ok)
        printf("PASS_460c33\n");
    return 0;
}