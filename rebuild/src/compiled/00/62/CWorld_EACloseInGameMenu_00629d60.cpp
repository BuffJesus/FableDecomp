#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_gui.h"
#include "fable_player_manager.h"
void CWorld::EACloseInGameMenu(const CGameEvent* event)
{
    SetAsPaused(false);
    GetPlayer(event->Player)->RemoveMode(PLAYER_MODE_NAVIGATE_IN_GAME_MENU);
    CPlayerGui::Get().GetPHeroInfoScreensManager()->Close();
}
