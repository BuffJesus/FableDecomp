// Behaviour fixture for CMainGameComponent::CreatePlayers @ 0x004166A8.
// PPlayerManager is +0x1C, Client +0x13AB8. Checks: the game definition
// manager is fetched first; the player manager creates the players; the
// client is initialised as local with this component; only when that fails
// is the client's IsValid asked, and only a valid client is uninitialised.
// Call order and receivers are checked; the component is untouched.
#include "fable_game.h"
#include "fable_definition_manager.h"
#include "fable_player_manager.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c) { static_cast<MainGameProbe*>(c)->CreatePlayers(); }
};

static unsigned char comp[0x161F0];
static unsigned char playerManager[0x2C];
static unsigned char defMgr[0xE0];
static int seq, getAt, createAt, initAt, validAt, uninitAt, calls;
static bool initResult, validResult, ok;

CGameDefinitionManager& FABLE_FASTCALL CGameDefinitionManager::Get()
{
    getAt = ++seq; ++calls;
    return *reinterpret_cast<CGameDefinitionManager*>(defMgr);
}
void CPlayerManager::CreatePlayers()
{
    ok = ok && static_cast<void*>(this) == playerManager;
    createAt = ++seq; ++calls;
}
bool CNetworkClient::InitialiseAsLocal(CMainGameComponent* component)
{
    ok = ok && static_cast<void*>(this) == comp + 0x13AB8 && static_cast<void*>(component) == comp;
    initAt = ++seq; ++calls;
    return initResult;
}
bool CInitBaseClass::IsValid() const
{
    ok = ok && static_cast<const void*>(this) == comp + 0x13AB8;
    validAt = ++seq; ++calls;
    return validResult;
}
void CNetworkClient::Uninitialise()
{
    ok = ok && static_cast<void*>(this) == comp + 0x13AB8;
    uninitAt = ++seq; ++calls;
}

static bool Run(bool initOk, bool valid)
{
    memset(comp, 0xE1, sizeof(comp));
    void* pm = playerManager; memcpy(comp + 0x1C, &pm, 4);
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    initResult = initOk; validResult = valid;
    seq = getAt = createAt = initAt = validAt = uninitAt = calls = 0;
    ok = true;
    MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp));
    int s = 0;
    ok = ok && getAt == ++s && createAt == ++s && initAt == ++s;
    if (!initOk)
    {
        ok = ok && validAt == ++s;
        if (valid) ok = ok && uninitAt == ++s; else ok = ok && uninitAt == 0;
    }
    else
        ok = ok && validAt == 0 && uninitAt == 0;
    ok = ok && calls == s && memcmp(before, comp, sizeof(comp)) == 0;
    if (!ok) printf("case init=%d valid=%d failed\n", initOk, valid);
    return ok;
}

int main()
{
    bool pass = Run(true, true);
    pass = Run(true, false) && pass;
    pass = Run(false, true) && pass;
    pass = Run(false, false) && pass;
    printf(pass ? "PASS_004166a8\n" : "FAIL_004166a8\n");
    return pass ? 0 : 1;
}
