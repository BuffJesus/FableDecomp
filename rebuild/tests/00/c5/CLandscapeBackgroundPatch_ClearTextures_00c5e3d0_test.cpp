#include <cstdio>

static int g_lastReleased;
static int g_releaseCalls;

void __stdcall ReleaseImpl(int handle)
{
    g_lastReleased = handle;
    g_releaseCalls++;
}
void (__stdcall *g_ReleaseTexture)(int handle) = ReleaseImpl;

struct CLandscapeBackgroundPatch
{
    int field_0;
    int textureHandle;
    void ClearTextures();
};

void CLandscapeBackgroundPatch::ClearTextures()
{
    int h = this->textureHandle;
    if (h != -1)
    {
        g_ReleaseTexture(h);
        this->textureHandle = -1;
    }
}

int main()
{
    int ok = 1;

    // Case 1: valid handle -> released and cleared
    CLandscapeBackgroundPatch a;
    a.field_0 = 0x11111111;
    a.textureHandle = 42;
    g_releaseCalls = 0; g_lastReleased = -999;
    a.ClearTextures();
    if (g_releaseCalls != 1) ok = 0;
    if (g_lastReleased != 42) ok = 0;
    if (a.textureHandle != -1) ok = 0;
    if (a.field_0 != 0x11111111) ok = 0;

    // Case 2: already -1 -> no call, stays -1
    CLandscapeBackgroundPatch b;
    b.field_0 = 0x22222222;
    b.textureHandle = -1;
    g_releaseCalls = 0;
    b.ClearTextures();
    if (g_releaseCalls != 0) ok = 0;
    if (b.textureHandle != -1) ok = 0;

    if (ok) printf("CLEARTEX_OK\n");
    else printf("CLEARTEX_FAIL\n");
    return ok ? 0 : 1;
}