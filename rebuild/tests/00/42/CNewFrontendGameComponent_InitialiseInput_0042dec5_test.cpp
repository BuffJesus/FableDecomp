#include <stdio.h>
#include <string.h>

#include "fable_game.h"
#include "fable_gfuninitialise.h"

// CSystemManager has string members whose constructors live elsewhere;
// the leaf only needs the object's address and fields.
static fable_u32 g_systemManagerStorage[sizeof(CSystemManager) / 4];
static CSystemManager& g_systemManager =
    *reinterpret_cast<CSystemManager*>(g_systemManagerStorage);
static int g_getSystemManagerCalls;

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    ++g_getSystemManagerCalls;
    return &g_systemManager;
}

int main()
{
    CInputManager* input = reinterpret_cast<CInputManager*>(0x13579BDFUL);
    g_systemManager.PInputManager.Data = input;
    g_systemManager.PInputManager.Info =
        reinterpret_cast<CCPPointerInfo*>(0x2468ACE0UL);

    // Raw storage: only the input-manager slot may change.
    unsigned long storage[sizeof(CNewFrontendGameComponent) / 4];
    memset(storage, 0xA5, sizeof(storage));
    unsigned long before[sizeof(storage) / 4];
    memcpy(before, storage, sizeof(storage));

    reinterpret_cast<CNewFrontendGameComponent*>(storage)->InitialiseInput();

    before[0x68 / 4] = 0x13579BDFUL;
    if (g_getSystemManagerCalls != 1 ||
        memcmp(before, storage, sizeof(storage)) != 0)
    {
        puts("FABLETLC_NEW_FRONTEND_INITIALISE_INPUT FAIL");
        return 1;
    }
    puts("PASS_0042dec5 FABLETLC_NEW_FRONTEND_INITIALISE_INPUT PASS");
    return 0;
}
