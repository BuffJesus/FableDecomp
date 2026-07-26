#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile { void* ptr; };

static int g_freed = 0;
static void* g_last = 0;
extern "C" void __cdecl fable_free_461009(void* p) { g_freed++; g_last = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

int main()
{
    // non-null case: callee invoked once with the pointer
    int dummy = 0;
    CActiveFile a; a.ptr = &dummy;
    CActiveFile_OnReadFinished(&a);

    // null case: callee not invoked
    CActiveFile b; b.ptr = 0;
    CActiveFile_OnReadFinished(&b);

    if (g_freed == 1 && g_last == &dummy)
        printf("OK_461009\n");
    else
        printf("FAIL_461009\n");
    return 0;
}