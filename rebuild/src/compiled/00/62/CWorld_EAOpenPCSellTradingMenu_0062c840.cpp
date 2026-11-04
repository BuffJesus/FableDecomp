#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
void CWorld::EAOpenPCSellTradingMenu(const CGameEvent* event)
{
    CTCInventoryTrade* ptc_inventory = 0;
    if (GetPlayer(event->Player)->GetPControlledCreature()->GetTC(&ptc_inventory))
        ptc_inventory->OpenActualSellInventoryPC();
}
