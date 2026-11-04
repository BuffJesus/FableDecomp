// lib_input_event.cpp, retail 0x00A03BC0-0x00A0415F: the CInputEvent setters
// in Ego_r order and with Ego_r parameter names. Each records the device, the
// event type, the event's data and its start/current times (floats from the
// double arguments). These supersede earlier landings under a stand-in
// `CInputEventRetail` class, which had a different mangled name.
#include "fable_input_event.h"

// Retail 00A03BC0.
void CInputEvent::SetMousePos(const C2DVector& pos)
{
    MousePos = pos;
}

// Retail 00A03BE0.
void CInputEvent::SetAsNull()
{
    Type = IE_NULL;
    DeviceType = INPUT_DEVICE_TYPE_NULL;
}

// Retail 00A03BF0.
void CInputEvent::SetAsKeyboardKeyPressed(EInputKey key, double time)
{
    Type = IE_KEY_PRESSED;
    CurrentTime = (float)time;
    StartTime = (float)time;
    DeviceType = INPUT_DEVICE_TYPE_KEYBOARD;
    Key = key;
}

// Retail 00A03C20.
void CInputEvent::SetAsKeyboardKeyHeld(EInputKey key, double start_time, double event_time)
{
    DeviceType = INPUT_DEVICE_TYPE_KEYBOARD;
    Type = IE_KEY_HELD;
    Key = key;
    StartTime = (float)start_time;
    CurrentTime = (float)event_time;
}

// Retail 00A03C50.
void CInputEvent::SetAsKeyboardKeyReleased(EInputKey key, double start_time, double event_time)
{
    DeviceType = INPUT_DEVICE_TYPE_KEYBOARD;
    Type = IE_KEY_RELEASED;
    Key = key;
    StartTime = (float)start_time;
    CurrentTime = (float)event_time;
}

// Retail 00A03C80.
void CInputEvent::SetAsLeftMouseButtonPressed(const C2DVector& pos, double time)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = IE_LMB_PRESSED;
    MousePos = pos;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A03CB0.
void CInputEvent::SetAsCharPressed(wchar_t tchar, double time)
{
    DeviceType = INPUT_DEVICE_TYPE_KEYBOARD;
    Type = IE_CHAR_PRESSED;
    Char = tchar;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A03CE0.
void CInputEvent::SetAsCharReleased(wchar_t tchar, double time)
{
    DeviceType = INPUT_DEVICE_TYPE_KEYBOARD;
    Type = IE_CHAR_RELEASED;
    Char = tchar;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A03D10.
void CInputEvent::SetAsLeftMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double start_time,
                                           double current_time)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = IE_LMB_HELD;
    MousePos = pos;
    StartTime = (float)start_time;
    CurrentTime = (float)current_time;
    HeldMouseMovementRef() = movement;
}

// Retail 00A03D60.
void CInputEvent::SetAsLeftMouseButtonReleased(const C2DVector& pos, double start_time, double current_time)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = IE_LMB_RELEASED;
    MousePos = pos;
    StartTime = (float)start_time;
    CurrentTime = (float)current_time;
}

// Retail 00A03D90.
void CInputEvent::SetAsMiddleMouseButtonPressed(const C2DVector& pos, double time)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = IE_MMB_PRESSED;
    MousePos = pos;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A03DC0.
void CInputEvent::SetAsMiddleMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double start_time,
                                             double current_time)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = IE_MMB_HELD;
    MousePos = pos;
    StartTime = (float)start_time;
    CurrentTime = (float)current_time;
    HeldMouseMovementRef() = movement;
}

// Retail 00A03E10.
void CInputEvent::SetAsMiddleMouseButtonReleased(const C2DVector& pos, double start_time, double current_time)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = IE_MMB_RELEASED;
    MousePos = pos;
    StartTime = (float)start_time;
    CurrentTime = (float)current_time;
}

// Retail 00A03E40.
void CInputEvent::SetAsRightMouseButtonPressed(const C2DVector& pos, double time)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = IE_RMB_PRESSED;
    MousePos = pos;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A03E70.
void CInputEvent::SetAsRightMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double start_time,
                                            double current_time)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = IE_RMB_HELD;
    MousePos = pos;
    StartTime = (float)start_time;
    CurrentTime = (float)current_time;
    HeldMouseMovementRef() = movement;
}

// Retail 00A03EC0.
void CInputEvent::SetAsRightMouseButtonReleased(const C2DVector& pos, double start_time, double current_time)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = IE_RMB_RELEASED;
    MousePos = pos;
    StartTime = (float)start_time;
    CurrentTime = (float)current_time;
}

// Retail 00A03EF0: extra buttons 0..4 are IE_MB4..IE_MB8.
void CInputEvent::SetAsExtraMouseButtonPressed(const C2DVector& pos, double time, long x)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = (EInputEventType)(IE_MB4_PRESSED + x);
    MousePos = pos;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A03F20.
void CInputEvent::SetAsExtraMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double start_time,
                                            double current_time, long x)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = (EInputEventType)(IE_MB4_HELD + x);
    MousePos = pos;
    StartTime = (float)start_time;
    CurrentTime = (float)current_time;
    HeldMouseMovementRef() = movement;
}

// Retail 00A03F70.
void CInputEvent::SetAsExtraMouseButtonReleased(const C2DVector& pos, double start_time, double current_time, long x)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = (EInputEventType)(IE_MB4_RELEASED + x);
    MousePos = pos;
    StartTime = (float)start_time;
    CurrentTime = (float)current_time;
}

// Retail 00A03FB0.
void CInputEvent::SetAsMouseMovement(const C2DVector& pos, const C3DVector& movement, double time)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = IE_MOUSE_MOVEMENT;
    MouseMovementRef() = movement;
    MousePos = pos;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A04000: the wheel is MouseMovement.Z.
void CInputEvent::SetAsMouseWheelMovement(float move, double time)
{
    DeviceType = INPUT_DEVICE_TYPE_MOUSE;
    Type = IE_MOUSE_WHEEL_MOVEMENT;
    MouseMovementStorage_[2] = move;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A04030.
void CInputEvent::SetAsJoystickPosition(const C2DVector& pos, double time)
{
    DeviceType = INPUT_DEVICE_TYPE_JOYSTICK;
    Type = IE_JOYSTICK_POSITION;
    JoystickPosStorage_[0] = pos.X;
    JoystickPosStorage_[1] = pos.Y;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A04060.
void CInputEvent::SetAsJoystickPosition2(const C2DVector& pos, double time)
{
    DeviceType = INPUT_DEVICE_TYPE_JOYSTICK;
    Type = IE_JOYSTICK_POSITION2;
    JoystickPosStorage_[0] = pos.X;
    JoystickPosStorage_[1] = pos.Y;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A04090.
void CInputEvent::SetAsJoystickButtonPressed(long button, float button_pressure_fraction, const C2DVector& pos,
                                             double time)
{
    JoystickButton = (unsigned char)button;
    Type = IE_JOYSTICK_BUTTON_PRESSED;
    DeviceType = INPUT_DEVICE_TYPE_JOYSTICK;
    JoystickPosStorage_[0] = pos.X;
    JoystickPosStorage_[1] = pos.Y;
    JoystickButtonPressureFraction = button_pressure_fraction;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A040D0.
void CInputEvent::SetAsJoystickButtonHeld(long button, float button_pressure_fraction, const C2DVector& pos,
                                          double time)
{
    JoystickButton = (unsigned char)button;
    Type = IE_JOYSTICK_BUTTON_HELD;
    DeviceType = INPUT_DEVICE_TYPE_JOYSTICK;
    JoystickPosStorage_[0] = pos.X;
    JoystickPosStorage_[1] = pos.Y;
    JoystickButtonPressureFraction = button_pressure_fraction;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A04110.
void CInputEvent::SetAsJoystickButtonReleased(long button, float button_pressure_fraction, const C2DVector& pos,
                                              double time)
{
    JoystickButton = (unsigned char)button;
    Type = IE_JOYSTICK_BUTTON_RELEASED;
    DeviceType = INPUT_DEVICE_TYPE_JOYSTICK;
    JoystickPosStorage_[0] = pos.X;
    JoystickPosStorage_[1] = pos.Y;
    JoystickButtonPressureFraction = button_pressure_fraction;
    StartTime = (float)time;
    CurrentTime = (float)time;
}

// Retail 00A04150.
void CInputEvent::SetJoystickButtonPressure(float button_pressure_fraction)
{
    JoystickButtonPressureFraction = button_pressure_fraction;
}
