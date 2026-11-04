// Behaviour fixture for CMainGameComponent::PeekWorldChecksum @ 0x00416392.
// Client.LocalGame is component +0x1611A; GPInputManager's Loading/Saving
// flags are +0x38/+0x39; PWorld is +0x24. Checks: a local game neither
// loading nor saving events returns 1 without asking the world; any other
// combination returns the world's checksum (asked once, on PWorld); nothing
// is written.
#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"
#include "fable_world.h"
#include <stdio.h>
#include <string.h>

CInputManager* g_FableInputManager_013B8388;

static unsigned char comp[0x161F0];
static unsigned char inputManager[0x84];
static unsigned char world[0x198];
static int checksumCalls;
static bool ok;

unsigned long CWorld::GetChecksum() const
{
    ok = ok && static_cast<const void*>(this) == world;
    ++checksumCalls;
    return 0xC0FFEE11;
}

static bool Run(bool local, bool loading, bool saving, unsigned long expect)
{
    g_FableInputManager_013B8388 = reinterpret_cast<CInputManager*>(inputManager);
    memset(comp, 0x81, sizeof(comp));
    memset(inputManager, 0x00, sizeof(inputManager));
    void* w = world;
    memcpy(comp + 0x24, &w, 4);
    comp[0x1611A] = local ? 1 : 0;
    inputManager[0x38] = loading ? 1 : 0;
    inputManager[0x39] = saving ? 1 : 0;
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    checksumCalls = 0;
    ok = true;
    unsigned long result = reinterpret_cast<CMainGameComponent*>(comp)->PeekWorldChecksum();
    ok = ok && result == expect && checksumCalls == (expect == 1 ? 0 : 1)
        && memcmp(before, comp, sizeof(comp)) == 0;
    if (!ok) printf("case local=%d loading=%d saving=%d failed\n", local, loading, saving);
    return ok;
}

int main()
{
    bool pass = Run(true, false, false, 1);
    pass = Run(false, false, false, 0xC0FFEE11) && pass;
    pass = Run(true, true, false, 0xC0FFEE11) && pass;
    pass = Run(true, false, true, 0xC0FFEE11) && pass;
    pass = Run(false, true, true, 0xC0FFEE11) && pass;
    printf(pass ? "PASS_00416392\n" : "FAIL_00416392\n");
    return pass ? 0 : 1;
}
