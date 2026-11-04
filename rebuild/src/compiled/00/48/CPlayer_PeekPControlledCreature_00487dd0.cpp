// CPlayer::PeekPControlledCreature @ 00487dd0; primary intelligent pointer +2C.
#include "fable_world.h"
#include "fable_player_manager.h"

const CThingPlayerCreature* CPlayer::PeekPControlledCreature() const
{
    return ControlledCreature.Get();
}
