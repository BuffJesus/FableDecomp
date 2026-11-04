#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
#include "fable_player_gui.h"
void CWorld::EACloseHeroInformationScreen(const CGameEvent* event)
{
    if (GetPlayer(event->Player)->HasMode(PLAYER_MODE_VIEW_HERO)) return;
    if (!IS_THING_ALIVE(GetPlayer(event->Player)->GetPControlledCreature())) return;
    CPlayerGui::Get().GetPHeroInfoScreensManager()->RemoveAllPlayerModes();
    if (GUnpauseAfterClosingHeroInformationScreen) SetAsPaused(false);
    GUnpauseAfterClosingHeroInformationScreen = true;
    CPlayerGui::Get().GetPHeroInfoScreensManager()->Close();
    CTCInventoryTrade* trade;
    if (GetPlayer(event->Player)->GetPControlledCreature()->GetTC(&trade))
        trade->ShowWarningIfNeeded();
}
