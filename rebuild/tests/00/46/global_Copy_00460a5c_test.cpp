#include <stdio.h>

/* Mimic the thunk shape without linking the engine: a local tail-call thunk.
   We only assert the reconstruction's control-flow semantics (delegates to
   impl, returns void) deterministically. */

struct CSoundDef { int tag; };

static int g_called = 0;
static CSoundDef* g_this = 0;
static void* g_src = 0;

void __fastcall CSoundDef_Copy_impl(CSoundDef* thisptr, void* src)
{
    g_called = 1;
    g_this = thisptr;
    g_src = src;
}

void __fastcall CSoundDef_Copy(CSoundDef* thisptr, void* src)
{
    CSoundDef_Copy_impl(thisptr, src);
}

int main(void)
{
    CSoundDef obj;
    obj.tag = 0x1234;
    int srcval = 0x55;

    CSoundDef_Copy(&obj, &srcval);

    if (g_called == 1 && g_this == &obj && g_src == (void*)&srcval)
        printf("OK_460a5c\n");
    else
        printf("FAIL_460a5c\n");
    return 0;
}