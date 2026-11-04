// CPlayer::GetPControlledCreature @ 00487dc0; primary intelligent pointer +2C.
#include "fable_world.h"
#include "fable_player_manager.h"

CThingPlayerCreature* CPlayer::GetPControlledCreature() const
{
    return ControlledCreature.Get();
}
