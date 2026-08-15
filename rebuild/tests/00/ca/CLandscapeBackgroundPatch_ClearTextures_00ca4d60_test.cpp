#include <stdio.h>

struct CLandscapeBackgroundPatch {
    char pad[0xc0];
    void* tex;
};

static int g_calls = 0;
static void* g_lastArg = 0;

static void __stdcall fake_release(void* p) {
    g_calls++;
    g_lastArg = p;
}
static void (__stdcall *g_releaseTexture)(void*) = fake_release;

static void __fastcall ClearTextures(CLandscapeBackgroundPatch* self)
{
    void* t = self->tex;
    if (t) {
        g_releaseTexture(t);
        self->tex = 0;
    }
}

int main() {
    CLandscapeBackgroundPatch a;
    a.tex = (void*)0x1234;
    ClearTextures(&a);
    int ok1 = (g_calls == 1) && (g_lastArg == (void*)0x1234) && (a.tex == 0);

    CLandscapeBackgroundPatch b;
    b.tex = 0;
    ClearTextures(&b);
    int ok2 = (g_calls == 1) && (b.tex == 0);

    if (ok1 && ok2) printf("CLEARTEX_OK\n");
    else printf("FAIL c=%d arg=%p\n", g_calls, g_lastArg);
    return 0;
}