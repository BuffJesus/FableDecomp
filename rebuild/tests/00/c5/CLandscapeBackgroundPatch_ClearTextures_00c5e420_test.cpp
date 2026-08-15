#include <stdio.h>

struct Tex { int alive; };

static int g_freeCalls = 0;
static Tex* g_lastFreed = 0;

struct Patch {
    void* vtbl;
    Tex*  tex;
};

static void freeTex(Tex* t) { g_freeCalls++; g_lastFreed = t; }

static void ClearTextures(Patch* self)
{
    Tex* t = self->tex;
    self->vtbl = (void*)0x12b90e8;
    if (t) {
        freeTex(t);
        self->tex = 0;
    }
}

int main()
{
    // Case 1: tex non-null -> freed and nulled, vtbl set
    Tex tx; tx.alive = 1;
    Patch p; p.vtbl = 0; p.tex = &tx;
    ClearTextures(&p);
    if (p.vtbl != (void*)0x12b90e8) { printf("bad vtbl\n"); return 1; }
    if (g_freeCalls != 1 || g_lastFreed != &tx) { printf("bad free\n"); return 1; }
    if (p.tex != 0) { printf("tex not nulled\n"); return 1; }

    // Case 2: tex null -> no free, vtbl still set, tex stays null
    Patch q; q.vtbl = 0; q.tex = 0;
    ClearTextures(&q);
    if (q.vtbl != (void*)0x12b90e8) { printf("bad vtbl2\n"); return 1; }
    if (g_freeCalls != 1) { printf("unexpected free\n"); return 1; }
    if (q.tex != 0) { printf("tex2\n"); return 1; }

    printf("CLEARTEX_OK\n");
    return 0;
}