// CJoystickDX::EnumAndInit (retail 0x00AB6B10): the IDirectInput8::EnumDevices callback CJoystickDX::SetExclusiveMode
// passes. It sets up the first attached game controller the game can use - joystick data format, exclusive
// foreground access, both main axes ranged 0..0x10000 with a 2000 dead zone, a 64-record input buffer - and acquires
// it; any step that fails releases that device and moves on to the next one.
#include "fable_dinput8.h"
#include "fable_input_manager.h"
#include "fable_joystick_dx.h"
#include "fable_gfuninitialise.h"

int __stdcall CJoystickDX::EnumAndInit(const DIDEVICEINSTANCEW* instance, void* context)
{
    CJoystickDX* joystick = static_cast<CJoystickDX*>(context);
    IDirectInputDevice8W* device;
    if (joystick->InputManagerDX->GetDInput()->CreateDevice(instance->guidInstance, &device, 0) != DI_OK)
        return DIENUM_CONTINUE;

    if (device->SetDataFormat(&c_dfDIJoystick) != DI_OK ||
        device->SetCooperativeLevel(GFGetSystemManager()->GetApplicationWinHandle(), DISCL_EXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
    {
        device->Release();
        return DIENUM_CONTINUE;
    }

    DIPROPRANGE range;
    range.diph.dwSize = sizeof(DIPROPRANGE);
    range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    range.diph.dwObj = DIJOFS_X;
    range.diph.dwHow = DIPH_BYOFFSET;
    range.lMin = 0;
    range.lMax = 0x10000;
    if (device->SetProperty(DIPROP_RANGE, &range.diph) != DI_OK)
    {
        device->Release();
        return DIENUM_CONTINUE;
    }
    range.diph.dwSize = sizeof(DIPROPRANGE);
    range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    range.diph.dwObj = DIJOFS_Y;
    range.diph.dwHow = DIPH_BYOFFSET;
    range.lMin = 0;
    range.lMax = 0x10000;
    if (device->SetProperty(DIPROP_RANGE, &range.diph) != DI_OK)
    {
        device->Release();
        return DIENUM_CONTINUE;
    }

    DIPROPDWORD deadZone;
    deadZone.diph.dwSize = sizeof(DIPROPDWORD);
    deadZone.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    deadZone.diph.dwObj = DIJOFS_X;
    deadZone.diph.dwHow = DIPH_BYOFFSET;
    deadZone.dwData = 2000;
    if (device->SetProperty(DIPROP_DEADZONE, &deadZone.diph) != DI_OK)
    {
        device->Release();
        return DIENUM_CONTINUE;
    }
    deadZone.diph.dwSize = sizeof(DIPROPDWORD);
    deadZone.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    deadZone.diph.dwObj = DIJOFS_Y;
    deadZone.diph.dwHow = DIPH_BYOFFSET;
    deadZone.dwData = 2000;
    if (device->SetProperty(DIPROP_DEADZONE, &deadZone.diph) != DI_OK)
    {
        device->Release();
        return DIENUM_CONTINUE;
    }

    DIPROPDWORD bufferSize;
    bufferSize.diph.dwSize = sizeof(DIPROPDWORD);
    bufferSize.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    bufferSize.diph.dwObj = 0;
    bufferSize.diph.dwHow = DIPH_DEVICE;
    bufferSize.dwData = 64;
    const HRESULT buffered = device->SetProperty(DIPROP_BUFFERSIZE, &bufferSize.diph);
    if (buffered != DI_OK && buffered != 2)
    {
        device->Release();
        return DIENUM_CONTINUE;
    }

    if (device->Acquire() != DI_OK)
    {
        device->Release();
        joystick->Acquired = false;
        return DIENUM_CONTINUE;
    }
    joystick->Acquired = true;
    joystick->DJoystick = device;
    return DIENUM_STOP;
}
