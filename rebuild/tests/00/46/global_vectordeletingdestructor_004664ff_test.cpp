#include "rebuild_abi.h"
#include <stdio.h>

struct CLandscapeBackgroundPatch { int dummy; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr)
{
    (void)thisptr;
    ++g_dtor_calls;
}
extern "C" void __cdecl fable_operator_delete(void* p)
{
    (void)p;
    ++g_delete_calls;
}

extern "C" void* __fastcall CLandscapeBackgroundPatch_vector_deleting_dtor(
    CLandscapeBackgroundPatch* thisptr, int edx, unsigned int flags);

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.dummy = 0x1234;

    // flags bit0 clear: dtor runs, no delete
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_dtor(&obj, 0, 0);
    // flags bit0 set: dtor runs, delete runs
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_dtor(&obj, 0, 1);

    if (r1 == &obj && r2 == &obj && g_dtor_calls == 2 && g_delete_calls == 1)
        printf("OK_004664ff\n");
    else
        printf("FAIL dtor=%d del=%d\n", g_dtor_calls, g_delete_calls);
    return 0;
}