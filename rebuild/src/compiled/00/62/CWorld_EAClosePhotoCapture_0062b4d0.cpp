#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
#include "fable_player_gui.h"
void CWorld::EAClosePhotoCapture(const CGameEvent* event)
{
    SetAsPaused(false);
    CPlayer* player = GetPlayer(event->Player);
    player->RemoveMode(PLAYER_MODE_TAKE_SCREENSHOT_FOR_PHOTOJOURNAL);
    CPlayerGui::Get().GetPHeroInfoScreensManager()->Close();
    CTCPhotojournalCapture* capture;
    if (player->GetPControlledCreature()->GetTC(&capture))
        capture->DestroyScreen();
}
