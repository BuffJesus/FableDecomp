// Behaviour fixture for CMainGameComponent::BeginInputLoading @ 0x0041627F.
// Checks: CInputManager::StreamInputsFromFile runs once on GPInputManager
// with GVersion and the caller's file name (by reference, not a copy);
// StreamInputsToFile is not called; the component is untouched.
#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c, const CWideString& f) { static_cast<MainGameProbe*>(c)->BeginInputLoading(f); }
};

CVersion g_Version_013B85E4;
CInputManager* g_FableInputManager_013B8388;

static unsigned char comp[0x161F0];
static unsigned char inputManager[0x84];
static unsigned char fileName[16];
static int toCalls, fromCalls;
static bool ok;

void CInputManager::StreamInputsFromFile(const CVersion& version, const CWideString& name)
{
    ++fromCalls;
    ok = ok && static_cast<void*>(this) == inputManager && &version == &g_Version_013B85E4
        && static_cast<const void*>(&name) == fileName;
}
void CInputManager::StreamInputsToFile(const CVersion&, const CWideString&) { ++toCalls; }

int main()
{
    g_FableInputManager_013B8388 = reinterpret_cast<CInputManager*>(inputManager);
    memset(comp, 0x4D, sizeof(comp));
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    ok = true;
    MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp), *reinterpret_cast<const CWideString*>(fileName));
    bool pass = ok && fromCalls == 1 && toCalls == 0 && memcmp(before, comp, sizeof(comp)) == 0;
    printf(pass ? "PASS_0041627f\n" : "FAIL_0041627f\n");
    return pass ? 0 : 1;
}
