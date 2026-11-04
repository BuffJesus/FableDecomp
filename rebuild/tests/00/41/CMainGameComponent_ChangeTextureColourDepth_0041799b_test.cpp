// Behaviour fixture for CMainGameComponent::ChangeTextureColourDepth @ 0x0041799B.
// PWorld is +0x24, PDisplayEngine +0x28. Checks: depths other than 16/24/32
// are ignored without asking the display manager; an unsupported depth, or
// the current GTextureBitDepth, changes nothing; otherwise GTextureBitDepth
// is updated before InitGraphics runs, then the display engine and the world
// are told the new depth, in that order. The component is untouched.
#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_world.h"
#include <stdio.h>
#include <string.h>

CDisplayManager* g_FableDisplayManager_013B8390;
unsigned long g_FableTextureBitDepth_013B7D68;

static unsigned char comp[0x161F0];
static unsigned long displayManager[0x2C8 / 4];
static unsigned char display[0x100];
static unsigned char world[0x198];
static int seq, supportAt, graphicsAt, engineAt, worldAt, calls;
static long seenDepth[3];
static bool supported, ok;

bool CDisplayManager::IsTextureBitDepthSupported(unsigned long bitDepth)
{
    ok = ok && static_cast<void*>(this) == displayManager;
    seenDepth[0] = (long)bitDepth;
    supportAt = ++seq; ++calls;
    return supported;
}
bool CMainGameComponent::InitGraphics()
{
    ok = ok && static_cast<void*>(this) == comp && g_FableTextureBitDepth_013B7D68 == (unsigned long)seenDepth[0];
    graphicsAt = ++seq; ++calls;
    return true;
}
void CDisplayEngine::ChangeTextureColourDepth(long depth)
{
    ok = ok && static_cast<void*>(this) == display;
    seenDepth[1] = depth;
    engineAt = ++seq; ++calls;
}
void CWorld::ChangeTextureColourDepth(long depth)
{
    ok = ok && static_cast<void*>(this) == world;
    seenDepth[2] = depth;
    worldAt = ++seq; ++calls;
}

static bool Run(long depth, bool isSupported, unsigned long current, bool expectQuery, bool expectChange)
{
    g_FableDisplayManager_013B8390 = reinterpret_cast<CDisplayManager*>(displayManager);
    g_FableTextureBitDepth_013B7D68 = current;
    memset(comp, 0x66, sizeof(comp));
    void* w = world; memcpy(comp + 0x24, &w, 4);
    void* d = display; memcpy(comp + 0x28, &d, 4);
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    supported = isSupported;
    seq = supportAt = graphicsAt = engineAt = worldAt = calls = 0;
    memset(seenDepth, 0, sizeof(seenDepth));
    ok = true;
    reinterpret_cast<CMainGameComponent*>(comp)->CMainGameComponent::ChangeTextureColourDepth(depth);
    int s = 0;
    if (expectQuery) ok = ok && supportAt == ++s && seenDepth[0] == depth; else ok = ok && supportAt == 0;
    if (expectChange)
        ok = ok && graphicsAt == ++s && engineAt == ++s && worldAt == ++s
            && seenDepth[1] == depth && seenDepth[2] == depth
            && g_FableTextureBitDepth_013B7D68 == (unsigned long)depth;
    else
        ok = ok && graphicsAt == 0 && engineAt == 0 && worldAt == 0 && g_FableTextureBitDepth_013B7D68 == current;
    ok = ok && calls == s && memcmp(before, comp, sizeof(comp)) == 0;
    if (!ok) printf("case depth=%ld supported=%d current=%lu failed\n", depth, isSupported, current);
    return ok;
}

int main()
{
    bool pass = Run(8, true, 32, false, false);
    pass = Run(15, true, 32, false, false) && pass;
    pass = Run(16, false, 32, true, false) && pass;
    pass = Run(32, true, 32, true, false) && pass;
    pass = Run(16, true, 32, true, true) && pass;
    pass = Run(24, true, 16, true, true) && pass;
    pass = Run(32, true, 24, true, true) && pass;
    printf(pass ? "PASS_0041799b\n" : "FAIL_0041799b\n");
    return pass ? 0 : 1;
}
