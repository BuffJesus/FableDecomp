// CJoystickDX's small overrides (retail 0x00AB6AD0..0x00AB6B00 and 0x00AB78D0..0x00AB7940): the four axes, setting
// the main axes, the per-frame update, and the joystick's empty force-feedback slots.
#include "fable_joystick_dx.h"

// 0x00AB6AD0
float CJoystickDX::GetXAxis()
{
    return XAxis;
}

// 0x00AB6AE0
float CJoystickDX::GetYAxis()
{
    return YAxis;
}

// 0x00AB6AF0
float CJoystickDX::GetXAxis2()
{
    return XAxis2;
}

// 0x00AB6B00
float CJoystickDX::GetYAxis2()
{
    return YAxis2;
}

// 0x00AB78D0: the base keeps maintained events alive, then the device's buffered input is read.
void CJoystickDX::Update()
{
    CJoystick::Update();
    UpdateEvents();
}

// 0x00AB78E0
void CJoystickDX::SetAxes(const C2DVector& axes)
{
    XAxis = axes.X;
    YAxis = axes.Y;
}

// 0x00AB7900
long CJoystickDX::GetJoystickDeviceNumber() const
{
    return 0;
}

// 0x00AB7910
CJoystick::CActiveRumble* CJoystickDX::GetRumbleWithID(long)
{
    return 0;
}

// 0x00AB7920
const CJoystick::CActiveRumble* CJoystickDX::PeekRumbleWithID(long) const
{
    return 0;
}

// 0x00AB7930
void CJoystickDX::AddRumble(long, const CInitRumble&, bool)
{
}

// 0x00AB7940
void CJoystickDX::ClearAllRumbles()
{
}
