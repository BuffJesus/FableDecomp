#include <cstdio>

typedef void (__stdcall *FreeFn)(void*);
extern FreeFn g_freeFn;

struct CLandscapeBackgroundPatch
{
    void* m_field0;
    void* m_textures;
    void ClearTextures();
};

void CLandscapeBackgroundPatch::ClearTextures()
{
    void* p = this->m_textures;
    if (p != 0)
    {
        g_freeFn(p);
        this->m_textures = 0;
    }
}

static int g_freed = 0;
static void* g_lastFreed = 0;

static void __stdcall myFree(void* p)
{
    g_freed++;
    g_lastFreed = p;
}

FreeFn g_freeFn = myFree;

int main()
{
    int ok = 1;

    // Case 1: non-null texture pointer -> freed and nulled
    int dummy = 0;
    CLandscapeBackgroundPatch a;
    a.m_field0 = (void*)0x1234;
    a.m_textures = &dummy;
    a.ClearTextures();
    if (g_freed != 1) ok = 0;
    if (g_lastFreed != &dummy) ok = 0;
    if (a.m_textures != 0) ok = 0;
    if (a.m_field0 != (void*)0x1234) ok = 0; // untouched

    // Case 2: null texture pointer -> no free call, stays null
    CLandscapeBackgroundPatch b;
    b.m_field0 = (void*)0xABCD;
    b.m_textures = 0;
    b.ClearTextures();
    if (g_freed != 1) ok = 0;   // unchanged
    if (b.m_textures != 0) ok = 0;

    if (ok)
        printf("CLEARTEX_OK\n");
    else
        printf("CLEARTEX_FAIL freed=%d\n", g_freed);
    return ok ? 0 : 1;
}