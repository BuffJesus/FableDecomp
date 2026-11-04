#pragma once
// Hand-owned (retail model): CJoystickDX, the DirectInput joystick. Ego_r.pdb names, members and access; retail RTTI
// vtable 0x0129EA7C fills CJoystick's slots (fable_input_devices.h): [3..6] GetXAxis/GetYAxis/GetXAxis2/GetYAxis2
// 0x00AB6AD0..0x00AB6B00, [7] SetAxes 0x00AB78E0, [8] GetJoystickDeviceNumber 0x00AB7900, [9] ??_G 0x00AB7950,
// [10] Update 0x00AB78D0, [12] SetExclusiveMode 0x00AB6DE0, [13]/[14] Get/PeekRumbleWithID 0x00AB7910/0x00AB7920,
// [16] SetHasInputFocus 0x00AB6D10, [17] AddRumble 0x00AB7930, [18] ClearAllRumbles 0x00AB7940 (retail's DirectInput
// joystick has no force feedback: the rumble slots do nothing). Ego_r CJoystickDX is 0xD4C over a 0xD20 CJoystick;
// retail's STLport list is one pointer, so CJoystick is 0xD18 and every member below sits 8 bytes lower (retail 0xD44).
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_2d_vector.h"
#include "fable_input_devices.h"

struct IDirectInputDevice8W;
class CInputManagerDX;

class CJoystickDX : public CJoystick
{
protected:
    virtual float GetXAxis();
    virtual float GetYAxis();
    virtual float GetXAxis2();
    virtual float GetYAxis2();

public:
    virtual void SetAxes(const C2DVector& axes);
    virtual long GetJoystickDeviceNumber() const;
    virtual ~CJoystickDX();
    virtual void Update();
    virtual bool SetExclusiveMode(bool exclusive);
    virtual CActiveRumble* GetRumbleWithID(long id);
    virtual const CActiveRumble* PeekRumbleWithID(long id) const;
    virtual void SetHasInputFocus(bool hasFocus);
    virtual void AddRumble(long id, const CInitRumble& rumble, bool replace);
    virtual void ClearAllRumbles();

    // Ego_r ?UpdateAcquired@CJoystickDX@@QAEXXZ (retail 0x00AB6D60): hold the device only while the application is
    // active and has input focus.
    void UpdateAcquired();

protected:
    // Ego_r ?EnumAndInit@CJoystickDX@@KGHPBUDIDEVICEINSTANCEW@@PAX@Z (retail 0x00AB6B10): the EnumDevices callback that
    // creates and configures the first attached game controller.
    static int __stdcall EnumAndInit(const struct DIDEVICEINSTANCEW* instance, void* joystick);

    // Ego_r ?UpdateEvents@CJoystickDX@@IAEXXZ, retail 0x00AB6E40.
    void UpdateEvents();

    friend struct CJoystickDXLayoutCheck;
    float XAxis;                                     // +0xD18
    float YAxis;                                     // +0xD1C
    float XAxis2;                                    // +0xD20
    float YAxis2;                                    // +0xD24
    long XAxisMin;                                   // +0xD28
    long XAxisMax;                                   // +0xD2C
    long YAxisMin;                                   // +0xD30
    long YAxisMax;                                   // +0xD34
    IDirectInputDevice8W* DJoystick;                 // +0xD38
    bool Acquired;                                   // +0xD3C
    CInputManagerDX* InputManagerDX;                 // +0xD40 (Ego_r: CInputManagerDX&)
};

struct CJoystickDXLayoutCheck
{
    FABLE_STATIC_ASSERT(offsetof(CJoystickDX, XAxis) == 0xD18);
    FABLE_STATIC_ASSERT(offsetof(CJoystickDX, YAxis2) == 0xD24);
    FABLE_STATIC_ASSERT(offsetof(CJoystickDX, DJoystick) == 0xD38);
    FABLE_STATIC_ASSERT(offsetof(CJoystickDX, InputManagerDX) == 0xD40);
};
FABLE_STATIC_ASSERT(sizeof(CJoystickDX) == 0xD44);
