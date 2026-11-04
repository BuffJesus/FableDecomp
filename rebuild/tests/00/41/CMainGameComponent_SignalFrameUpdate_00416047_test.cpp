// Behaviour fixture for CMainGameComponent::SignalFrameUpdate @ 0x00416047.
// Checks: FirstWorldFrameUpdate (+0x44) is cleared before the allocator's
// NewFrame runs; NewFrame runs once on the allocator singleton; every other
// component byte is untouched.
#include "fable_game.h"
#include "fable_memory_allocator.h"
#include <stdio.h>
#include <string.h>

CMemoryAllocator* g_FableMemoryAllocator_013CAA90;

static unsigned char comp[0x161F0];
static unsigned char allocator[0x420];
static int newFrameCalls;
static bool ok;

void CMemoryAllocator::NewFrame()
{
    ++newFrameCalls;
    ok = ok && static_cast<void*>(this) == allocator && comp[0x44] == 0;
}

static bool Run(unsigned char initial)
{
    memset(comp, initial, sizeof(comp));
    comp[0x44] = 1;
    static unsigned char expect[sizeof(comp)];
    memcpy(expect, comp, sizeof(comp));
    expect[0x44] = 0;
    g_FableMemoryAllocator_013CAA90 = reinterpret_cast<CMemoryAllocator*>(allocator);
    newFrameCalls = 0;
    ok = true;
    reinterpret_cast<CMainGameComponent*>(comp)->SignalFrameUpdate();
    ok = ok && newFrameCalls == 1 && memcmp(expect, comp, sizeof(comp)) == 0;
    if (!ok) printf("case %02x failed\n", initial);
    return ok;
}

int main()
{
    bool pass = Run(0xA5);
    pass = Run(0x00) && pass;
    printf(pass ? "PASS_00416047\n" : "FAIL_00416047\n");
    return pass ? 0 : 1;
}
