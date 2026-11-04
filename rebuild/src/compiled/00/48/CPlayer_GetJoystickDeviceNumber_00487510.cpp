// CPlayer::GetJoystickDeviceNumber (retail 0x00487510, Ego_r 0x00492D00): the controller this player reads input from.
#include "fable_player_manager.h"

long CPlayer::GetJoystickDeviceNumber() const
{
    return JoystickDeviceNumber;
}
