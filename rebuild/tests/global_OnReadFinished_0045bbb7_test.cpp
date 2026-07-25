#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile { void* field_0; };
extern "C" void __cdecl sub_7a2e5d(void* p);

static int g_called = 0;
static void* g_arg = 0;
extern "C" void __cdecl sub_7a2e5d(void* p) { g_called++; g_arg = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

int main()
{
    // null member -> no call
    CActiveFile a; a.field_0 = 0;
    CActiveFile_OnReadFinished(&a);
    // non-null member -> call with the pointer
    int dummy = 42;
    CActiveFile b; b.field_0 = &dummy;
    CActiveFile_OnReadFinished(&b);

    if (g_called == 1 && g_arg == &dummy)
        printf("OK_0045bbb7\n");
    else
        printf("FAIL_0045bbb7\n");
    return 0;
}