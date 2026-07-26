#include "rebuild_abi.h"
#include <cstdio>

struct CLandscapeBackgroundPatch
{
    fable_u32 vtbl;
};

extern void FABLE_FASTCALL CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr);
extern void FABLE_CDECL CLandscapeBackgroundPatch_opDelete(void* block);
extern void* FABLE_FASTCALL CLandscapeBackgroundPatch_vector_deleting_dtor(
    CLandscapeBackgroundPatch* thisptr, int edx_unused, fable_u32 flags);

static int g_dtorCalls = 0;
static int g_deleteCalls = 0;
static void* g_deletedBlock = 0;

void FABLE_FASTCALL CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr)
{
    (void)thisptr;
    ++g_dtorCalls;
}

void FABLE_CDECL CLandscapeBackgroundPatch_opDelete(void* block)
{
    ++g_deleteCalls;
    g_deletedBlock = block;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.vtbl = 0;

    // flags bit0 clear: dtor runs, no delete, returns this.
    g_dtorCalls = g_deleteCalls = 0; g_deletedBlock = 0;
    void* r0 = CLandscapeBackgroundPatch_vector_deleting_dtor(&obj, 0, 0);
    int ok0 = (r0 == &obj) && (g_dtorCalls == 1) && (g_deleteCalls == 0);

    // flags bit0 set: dtor runs, delete runs on this, returns this.
    g_dtorCalls = g_deleteCalls = 0; g_deletedBlock = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_dtor(&obj, 0, 1);
    int ok1 = (r1 == &obj) && (g_dtorCalls == 1) && (g_deleteCalls == 1) && (g_deletedBlock == &obj);

    // even flags (bit0 clear) should not delete.
    g_dtorCalls = g_deleteCalls = 0; g_deletedBlock = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_dtor(&obj, 0, 2);
    int ok2 = (r2 == &obj) && (g_dtorCalls == 1) && (g_deleteCalls == 0);

    if (ok0 && ok1 && ok2)
        printf("PASS_0046f483_VDDTOR\n");
    else
        printf("FAIL_0046f483 %d %d %d\n", ok0, ok1, ok2);
    return 0;
}