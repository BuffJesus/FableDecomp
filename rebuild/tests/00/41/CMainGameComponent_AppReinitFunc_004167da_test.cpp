// Behaviour fixture for CMainGameComponent::AppReinitFunc @ 0x004167DA.
// Called as the static fastcall callback with the component as context.
// PDisplayEngine is +0x28; GPDisplayManager's back buffer is at +0x2C.
// Checks: the display engine's PreChangeResolution runs first, then
// PostChangeResolution on the context component with the back buffer's
// width (GetDimensions().X), height (GetHeight) and pixel-format colour depth;
// every surface query is on the display manager's back buffer; the component
// is untouched.
#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_surface.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static void Call(void* context) { AppReinitFunc(context); }
};

CDisplayManager* g_FableDisplayManager_013B8390;

static unsigned char comp[0x161F0];
static unsigned long displayManager[0x2C8 / 4];
static unsigned char display[0x100];
static int seq, preAt, dimsAt, heightAt, pfAt, depthAt, postAt, calls;
static unsigned long gotW, gotH, gotD;
static bool ok;

static const void* BackBuffer() { return reinterpret_cast<unsigned char*>(displayManager) + 0x2C; }

void CDisplayEngine::PreChangeResolution()
{
    ok = ok && static_cast<void*>(this) == display;
    preAt = ++seq; ++calls;
}
C2DExtentsI CSurface::GetDimensions() const
{
    ok = ok && static_cast<const void*>(this) == BackBuffer();
    dimsAt = ++seq; ++calls;
    return C2DExtentsI(1280, 999);
}
unsigned long CSurface::GetHeight() const
{
    ok = ok && static_cast<const void*>(this) == BackBuffer();
    heightAt = ++seq; ++calls;
    return 720;
}
CPixelFormat CSurface::GetPixelFormat() const
{
    ok = ok && static_cast<const void*>(this) == BackBuffer();
    pfAt = ++seq; ++calls;
    CPixelFormat format;
    format.FormatIndex = 17;
    return format;
}
unsigned long CPixelFormat::GetColourDepth() const
{
    ok = ok && FormatIndex == 17;
    depthAt = ++seq; ++calls;
    return 32;
}
void CMainGameComponent::PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth)
{
    ok = ok && static_cast<void*>(this) == comp;
    gotW = width; gotH = height; gotD = depth;
    postAt = ++seq; ++calls;
}

int main()
{
    g_FableDisplayManager_013B8390 = reinterpret_cast<CDisplayManager*>(displayManager);
    memset(comp, 0x37, sizeof(comp));
    void* d = display; memcpy(comp + 0x28, &d, 4);
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    ok = true;
    MainGameProbe::Call(comp);
    bool pass = ok && preAt == 1 && postAt == calls && calls == 6
        && dimsAt > preAt && heightAt > preAt && pfAt > preAt && depthAt == pfAt + 1
        && gotW == 1280 && gotH == 720 && gotD == 32
        && memcmp(before, comp, sizeof(comp)) == 0;
    printf(pass ? "PASS_004167da\n" : "FAIL_004167da\n");
    return pass ? 0 : 1;
}
