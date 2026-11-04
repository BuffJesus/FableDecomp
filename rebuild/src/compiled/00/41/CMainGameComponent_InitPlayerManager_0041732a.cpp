// CMainGameComponent::InitPlayerManager @ 0x0041732A (48 bytes).
// FableWin ?InitPlayerManager@CMainGameComponent@@IAEXXZ. Replaces the
// player manager with a new one bound to this component and the game
// definition manager.
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

void CMainGameComponent::InitPlayerManager()
{
    PPlayerManager.reset(new CPlayerManager(*this, PeekDefinitionManager()));
}
