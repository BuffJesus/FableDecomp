#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
void CWorld::EARemoveCurrentMode(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    CThingPlayerCreature* creature = player->GetPControlledCreature();
    CGameEventDataPackage data;
    event->GetDataPackage(&data);
    player->RemoveMode(static_cast<EPlayerMode>(data.GetSLONG()));
    if (IS_THING_ALIVE(creature)) {
        CTCHeroRangedAttacks* ranged;
        if (creature->GetTC(&ranged)) ranged->ResetProjectileWeaponMode();
    }
}
