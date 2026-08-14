// Independent behaviour model for 0x00c9fdd0 _Cons_val fill-construct.
#include <stdio.h>
#include <string.h>

typedef unsigned long ulong;

static int g_initCalls = 0;
static void *g_lastArg = 0;

static void __cdecl initObject(void *obj)
{
    ++g_initCalls;
    g_lastArg = obj;
}

static void (__cdecl *g_initObject)(void *) = initObject;

struct C3DGeneratorObject {
    ulong words[0x1c5];
};

static void __fastcall Cons_val(C3DGeneratorObject *self)
{
    if (self == 0)
        return;
    ulong *p = self->words;
    for (int i = 0; i < 0x1c5; ++i)
        p[i] = 0;
    g_initObject((void *)self);
}

int main()
{
    // Null path: no init call, no crash.
    Cons_val(0);
    if (g_initCalls != 0) { printf("FAIL null\n"); return 1; }

    // Non-null: fill with garbage, verify zeroed + init called once with self.
    C3DGeneratorObject obj;
    memset(&obj, 0xAB, sizeof(obj));
    Cons_val(&obj);

    if (g_initCalls != 1) { printf("FAIL initcount %d\n", g_initCalls); return 1; }
    if (g_lastArg != (void *)&obj) { printf("FAIL arg\n"); return 1; }
    for (int i = 0; i < 0x1c5; ++i)
        if (obj.words[i] != 0) { printf("FAIL notzero %d\n", i); return 1; }

    if (sizeof(obj) != 1812) { printf("FAIL size %d\n", (int)sizeof(obj)); return 1; }

    printf("BEHAVIOUR_OK_c9fdd0\n");
    return 0;
}