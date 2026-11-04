// CMainGameComponent::CreatePlayers @ 0x004166A8 (58 bytes).
// FableWin ?CreatePlayers@CMainGameComponent@@IAEXXZ. Has the player manager
// create the players, then starts the local network client; if that fails on
// a client that did initialise, it is shut down again. (Retail keeps an
// out-of-line CGameDefinitionManager::Get call whose result is unused, as
// Ego_r's 0x0041596E does.)
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_definition_manager.h"
#include "fable_player_manager.h"

// ?PeekDefinitionManager@CMainGameComponent@@QBEABVCGameDefinitionManager@@XZ;
// inline in retail.
inline const CGameDefinitionManager& CMainGameComponent::PeekDefinitionManager() const
{
    return CGameDefinitionManager::Get();
}

void CMainGameComponent::CreatePlayers()
{
    PeekDefinitionManager();
    PPlayerManager->CreatePlayers();
    if (!Client.InitialiseAsLocal(this) && Client.IsValid())
        Client.Uninitialise();
}
