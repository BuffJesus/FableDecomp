#include "rebuild_abi.h"
#include <stdio.h>

struct CLandscapeBackgroundPatch { int dummy; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr)
{
    (void)thisptr;
    ++g_dtor_calls;
}

void __cdecl fable_operator_delete(void* p)
{
    (void)p;
    ++g_delete_calls;
}

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* thisptr, int edx, unsigned int flags);

int main(void)
{
    CLandscapeBackgroundPatch obj;
    obj.dummy = 42;

    // flags bit0 clear: dtor only, no delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r0 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);

    // flags bit0 set: dtor + delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);

    if (r0 == (void*)&obj && r1 == (void*)&obj &&
        g_dtor_calls == 1 && g_delete_calls == 1)
    {
        printf("OK_46ef6b\n");
    }
    else
    {
        printf("FAIL_46ef6b\n");
    }
    return 0;
}