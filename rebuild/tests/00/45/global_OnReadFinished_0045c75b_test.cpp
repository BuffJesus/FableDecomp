#include <cstdio>
#include "rebuild_abi.h"

struct CActiveFile
{
    void* m_ptr;
};

extern "C" void FABLE_CDECL sub_7a22b9(void* p);

void FABLE_FASTCALL OnReadFinished_0045c75b(CActiveFile* thisptr);

static int g_calls = 0;
static void* g_last = 0;

// Provide the callee for the test harness.
extern "C" void FABLE_CDECL sub_7a22b9(void* p)
{
    g_calls++;
    g_last = p;
}

int main()
{
    int sentinel = 42;

    // Case 1: null member -> callee must NOT be called.
    CActiveFile a;
    a.m_ptr = 0;
    OnReadFinished_0045c75b(&a);

    // Case 2: non-null member -> callee called once with that pointer.
    CActiveFile b;
    b.m_ptr = &sentinel;
    OnReadFinished_0045c75b(&b);

    if (g_calls == 1 && g_last == &sentinel)
    {
        printf("OK_0045c75b\n");
    }
    else
    {
        printf("FAIL_0045c75b calls=%d\n", g_calls);
    }
    return 0;
}