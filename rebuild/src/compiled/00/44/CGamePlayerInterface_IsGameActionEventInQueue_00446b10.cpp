// CGamePlayerInterface::IsGameActionEventInQueue without the time output (retail 0x00446B10, Ego_r 0x00443F70): ask the
// full query (0x00445CB0) and discard the event time.
#include "fable_player_manager.h"

bool CGamePlayerInterface::IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long joystick) const
{
    float time = 0.0f;
    return IsGameActionEventInQueue(action, type, joystick, time);
}
