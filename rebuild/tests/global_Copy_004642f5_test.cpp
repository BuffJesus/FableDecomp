#include <cstdio>

// Standalone build of the thunk-shaped function for a smoke test.
// (Real link forwards to the engine impl; here we stub it to observe forwarding.)

struct CThingObjectDef { int tag; };

static int g_calls = 0;
static CThingObjectDef* g_this = 0;
static CThingObjectDef* g_src = 0;

void __fastcall CThingObjectDef_Copy_impl(CThingObjectDef* thisptr, CThingObjectDef* src)
{
    g_calls++;
    g_this = thisptr;
    g_src = src;
}

void __fastcall CThingObjectDef_Copy(CThingObjectDef* thisptr, CThingObjectDef* src)
{
    CThingObjectDef_Copy_impl(thisptr, src);
}

int main()
{
    CThingObjectDef dst; dst.tag = 1;
    CThingObjectDef src; src.tag = 2;
    CThingObjectDef_Copy(&dst, &src);
    if (g_calls == 1 && g_this == &dst && g_src == &src)
        printf("OK_004642f5_THUNK\n");
    else
        printf("FAIL\n");
    return 0;
}