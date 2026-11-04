#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
void CWorld::EAOpenPCSkillsMenu(const CGameEvent* event)
{
    CTCInventory* ptc_inventory = 0;
    if (GetPlayer(event->Player)->GetPControlledCreature()->GetTC(&ptc_inventory))
        ptc_inventory->OpenPCSkillsMenu();
}
