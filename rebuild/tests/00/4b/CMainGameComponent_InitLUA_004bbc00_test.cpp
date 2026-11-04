// Behaviour fixture for CMainGameComponent::InitLUA @ 0x004BBC00.
// The retail body is empty (`ret 4`). Checks: the call returns with the
// component, the script-name string and the caller's stack intact (a
// wrong-arity body would unbalance the stack across the call).
#include "fable_game.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c, const CCharString& s) { static_cast<MainGameProbe*>(c)->InitLUA(s); }
};

static unsigned char comp[0x161F0];
static unsigned char name[16];

int main()
{
    memset(comp, 0x7E, sizeof(comp));
    memset(name, 0x21, sizeof(name));
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    volatile unsigned long guardA = 0xA11CE5ED, guardB = 0x0DDBA11;
    for (int i = 0; i < 3; ++i)
        MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp), *reinterpret_cast<const CCharString*>(name));
    bool pass = guardA == 0xA11CE5ED && guardB == 0x0DDBA11 && memcmp(before, comp, sizeof(comp)) == 0;
    for (int j = 0; j < 16; ++j) pass = pass && name[j] == 0x21;
    printf(pass ? "OK_004bbc00\n" : "FAIL_004bbc00\n");
    return pass ? 0 : 1;
}
