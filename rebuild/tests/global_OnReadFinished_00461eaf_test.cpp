#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile {
    void* m_ptr;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;

extern "C" void __cdecl free_461eaf(void* p);
void __fastcall OnReadFinished_461eaf(CActiveFile* thisptr);

extern "C" void __cdecl free_461eaf(void* p)
{
    g_freed = 1;
    g_freed_ptr = p;
}

int main()
{
    // Case 1: non-null pointer -> callee invoked
    int dummy = 0;
    CActiveFile a;
    a.m_ptr = &dummy;
    g_freed = 0; g_freed_ptr = 0;
    OnReadFinished_461eaf(&a);

    // Case 2: null pointer -> callee NOT invoked
    CActiveFile b;
    b.m_ptr = 0;
    int freed_after = 0;
    // reset and call
    g_freed = 0; g_freed_ptr = 0;
    OnReadFinished_461eaf(&b);
    freed_after = g_freed;

    if (g_freed_ptr == 0 /*unchanged from case2*/ && freed_after == 0) {
        // verify case1 separately
        CActiveFile c;
        c.m_ptr = &dummy;
        g_freed = 0; g_freed_ptr = 0;
        OnReadFinished_461eaf(&c);
        if (g_freed == 1 && g_freed_ptr == &dummy) {
            printf("PASS_461eaf_OK\n");
        }
    }
    return 0;
}