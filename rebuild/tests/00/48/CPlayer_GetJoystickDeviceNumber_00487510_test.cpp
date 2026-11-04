// Behaviour fixture for CPlayer::GetJoystickDeviceNumber (provider CPlayer_GetJoystickDeviceNumber_00487510.cpp): it must
// return the player's device number stored at retail +0x228 (raw storage; the player's constructor is not linked).
#include <stdio.h>
#include <string.h>
#include "fable_player_manager.h"

int main()
{
    static unsigned char raw[0x240];
    memset(raw, 0xEE, sizeof(raw));
    long device = 3;
    memcpy(raw + 0x228, &device, 4);
    const CPlayer* player = reinterpret_cast<const CPlayer*>(raw);
    long first = player->GetJoystickDeviceNumber();
    device = -1; memcpy(raw + 0x228, &device, 4);
    long second = player->GetJoystickDeviceNumber();
    if (first != 3 || second != -1) { printf("PLAYER_JOYSTICK_DEVICE_FAIL %ld %ld\n", first, second); return 1; }
    printf("PLAYER_JOYSTICK_DEVICE_OK\n");
    return 0;
}
