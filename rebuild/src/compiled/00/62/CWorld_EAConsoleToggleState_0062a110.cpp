#include "fable_world.h"
#include "fable_game.h"
#include "fable_player_manager.h"
#include "fable_console.h"
void CWorld::EAConsoleToggleState(const CGameEvent* event)
{
    CPlayer& player = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer();
    if (CConsole::Get().IsActive())
        player.RemoveMode(PLAYER_MODE_CONSOLE);
    else
        player.AddMode(PLAYER_MODE_CONSOLE, false);
}
