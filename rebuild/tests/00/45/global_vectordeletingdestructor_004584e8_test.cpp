#include "rebuild_abi.h"
#include <stdio.h>

struct CLandscapeBackgroundPatch { int dummy; };

static int g_dtor_called = 0;
static int g_delete_called = 0;

void FABLE_FASTCALL CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr)
{ (void)thisptr; g_dtor_called++; }

void FABLE_CDECL fable_operator_delete(void* p)
{ (void)p; g_delete_called++; }

extern void* FABLE_FASTCALL CLandscapeBackgroundPatch_vector_dtor(
    CLandscapeBackgroundPatch* thisptr, int edxDummy, fable_u32 flags);

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.dummy = 0;

    g_dtor_called = 0; g_delete_called = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_dtor(&obj, 0, 0);

    g_dtor_called = 0; g_delete_called = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_dtor(&obj, 0, 1);

    if (r1 == &obj && r2 == &obj && g_dtor_called == 1 && g_delete_called == 1)
        printf("PASS_004584e8_OK\n");
    else
        printf("FAIL\n");
    return 0;
}