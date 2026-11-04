// Behaviour fixture for CMainGameComponent::OnPostDeviceReset @ 0x004175DA.
// The override is entered with `this` on the CDeviceResetCallback subobject
// (component +0x04) and adjusts back. Checks: InitialiseFonts runs once on
// the full component; the result is true; nothing is written.
#include "fable_game.h"
#include <stdio.h>
#include <string.h>

static unsigned char comp[0x161F0];
static int fontCalls;
static bool ok;

void CMainGameComponent::InitialiseFonts()
{
    ok = ok && static_cast<void*>(this) == comp;
    ++fontCalls;
}

int main()
{
    memset(comp, 0x2A, sizeof(comp));
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    ok = true;
    CMainGameComponent* component = reinterpret_cast<CMainGameComponent*>(comp);
    // Qualified call: direct (non-virtual) entry, with the compiler applying
    // the +0x04 adjustment to the CDeviceResetCallback subobject.
    bool result = component->CMainGameComponent::OnPostDeviceReset();
    bool pass = ok && result && fontCalls == 1 && memcmp(before, comp, sizeof(comp)) == 0;
    printf(pass ? "PASS_004175da\n" : "FAIL_004175da\n");
    return pass ? 0 : 1;
}
