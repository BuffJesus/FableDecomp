#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile {
    void* m_ptr;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;

extern "C" void __cdecl EngineFree_460ea7(void* p)
{
    g_freed = 1;
    g_freed_ptr = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;
    if (p != 0) {
        EngineFree_460ea7(p);
    }
}

int main()
{
    // Case 1: non-null ptr -> callee invoked with that ptr
    int dummy = 0;
    CActiveFile a;
    a.m_ptr = &dummy;
    g_freed = 0; g_freed_ptr = 0;
    CActiveFile_OnReadFinished(&a);

    // Case 2: null ptr -> callee NOT invoked
    CActiveFile b;
    b.m_ptr = 0;
    int freed_on_null_probe = 0;
    g_freed = 0;
    CActiveFile_OnReadFinished(&b);
    freed_on_null_probe = g_freed;

    if (g_freed_ptr == &dummy && freed_on_null_probe == 0) {
        printf("OK_460ea7\n");
    } else {
        printf("FAIL_460ea7\n");
    }
    return 0;
}