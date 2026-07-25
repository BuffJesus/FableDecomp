#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile {
    void* ptr0;
};

static int g_called = 0;
static void* g_arg = 0;

extern "C" void __cdecl free_active_ptr(void* p) {
    g_called++;
    g_arg = p;
}

void __fastcall OnReadFinished_00462f42(CActiveFile* thisptr);

int main()
{
    // non-null path
    int dummy = 123;
    CActiveFile a;
    a.ptr0 = &dummy;
    OnReadFinished_00462f42(&a);

    // null path -> no call
    CActiveFile b;
    b.ptr0 = 0;
    OnReadFinished_00462f42(&b);

    if (g_called == 1 && g_arg == &dummy) {
        printf("OK_462f42\n");
    } else {
        printf("FAIL_462f42\n");
    }
    return 0;
}