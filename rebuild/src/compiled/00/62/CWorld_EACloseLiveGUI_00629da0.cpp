#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_gui.h"
#include "fable_player_manager.h"
void CWorld::EACloseLiveGUI(const CGameEvent* event)
{
    SetAsPaused(false);
    GetPlayer(event->Player)->RemoveMode(PLAYER_MODE_LIVE_GUI);
    CPlayerGui::Get().GetPHeroInfoScreensManager()->Close();
}
