// Behaviour fixture for CMainGameComponent::SetExclusiveMode @ 0x004163B7.
// Checks: the display engine's PreChangeResolution runs first, the flag is
// forwarded to the system manager, then the back buffer's width / colour
// depth / height are read and handed to PostChangeResolution(width, height,
// depth) on this component; no component field changes. Both flag values.
#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_gfuninitialise.h"
#include "fable_surface.h"
#include <stdio.h>
#include <string.h>

CDisplayManager* g_FableDisplayManager_013B8390;

static int seq;
static bool ok;
static unsigned char compStorage[0x161E8 + 16];
static unsigned char* comp = compStorage;
static unsigned long displayObject[0x2C8 / 4];
static unsigned char engineStorage[0x100];
static unsigned char systemStorage[0x180];
static const void* backBuffer = reinterpret_cast<unsigned char*>(displayObject) + 0x2C;

static int preAt, setAt, dimsAt, pfAt, depthAt, heightAt, postAt;
static bool setArg;
static unsigned long postW, postH, postD;

void CDisplayEngine::PreChangeResolution()
{
    preAt = ++seq;
    ok = ok && static_cast<void*>(this) == engineStorage;
}
CSystemManager* FABLE_FASTCALL GFGetSystemManager() { return reinterpret_cast<CSystemManager*>(systemStorage); }
bool CSystemManager::SetExclusiveMode(bool exclusive)
{
    setAt = ++seq;
    setArg = exclusive;
    ok = ok && static_cast<void*>(this) == systemStorage;
    return true;
}
C2DExtentsI CSurface::GetDimensions() const
{
    dimsAt = ++seq;
    ok = ok && static_cast<const void*>(this) == backBuffer;
    return C2DExtentsI(1280, 4444);
}
unsigned long CSurface::GetHeight() const
{
    heightAt = ++seq;
    ok = ok && static_cast<const void*>(this) == backBuffer;
    return 720;
}
CPixelFormat CSurface::GetPixelFormat() const
{
    pfAt = ++seq;
    ok = ok && static_cast<const void*>(this) == backBuffer;
    CPixelFormat f;
    f.FormatIndex = 31;
    return f;
}
unsigned long CPixelFormat::GetColourDepth() const
{
    depthAt = ++seq;
    ok = ok && FormatIndex == 31;
    return 32;
}
void CMainGameComponent::PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth)
{
    postAt = ++seq;
    ok = ok && static_cast<void*>(this) == comp;
    postW = width; postH = height; postD = depth;
}

static bool Run(bool exclusive)
{
    memset(compStorage, 0xA5, sizeof(compStorage));
    unsigned long e = (unsigned long)engineStorage;
    memcpy(comp + 0x28, &e, 4);
    unsigned char before[sizeof(compStorage)];
    memcpy(before, compStorage, sizeof(compStorage));
    g_FableDisplayManager_013B8390 = reinterpret_cast<CDisplayManager*>(displayObject);
    seq = preAt = setAt = dimsAt = pfAt = depthAt = heightAt = postAt = 0;
    setArg = !exclusive;
    postW = postH = postD = 0;
    ok = true;

    reinterpret_cast<CMainGameComponent*>(comp)->SetExclusiveMode(exclusive);

    if (preAt != 1 || setAt != 2 || postAt != 7) ok = false;
    // The three back-buffer reads happen between the switch and the post hook
    // (width, then colour depth, then height).
    if (!(dimsAt == 3 && pfAt == 4 && depthAt == 5 && heightAt == 6)) ok = false;
    if (setArg != exclusive) ok = false;
    if (postW != 1280 || postH != 720 || postD != 32) ok = false;
    if (memcmp(before, compStorage, sizeof(compStorage)) != 0) ok = false;
    return ok;
}

int main()
{
    bool pass = Run(true);
    pass = Run(false) && pass;
    if (pass)
        printf("FABLETLC_MAINGAME_SETEXCLUSIVEMODE PASS\n");
    else
        printf("FABLETLC_MAINGAME_SETEXCLUSIVEMODE FAIL\n");
    return pass ? 0 : 1;
}