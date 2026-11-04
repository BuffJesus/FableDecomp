#pragma once

// CInputEvent / CInputEventScanInfo / CInputManager event scanning and
// CGameJoystickManager, as CNewFrontendGameComponent::Input (0x0042E3EE) sees
// them. Names and signatures are FableWin.pdb's; enum values are FableWin.pdb's
// (llvm-pdbutil pretty --enums); layouts are retail's.
//
// Not to be included together with the generated POD views
// engine/CInputEvent.h / engine/CGameJoystickManager.h (same class names).

#include "fable_2d_vector.h"
#include "fable_base_class.h"
#include "fable_console.h"   // EInputKey
#include "fable_counted_pointer.h"
#include "rebuild_abi.h"

// FableWin.pdb enum EInputEventType.
enum EInputEventType
{
    IE_NULL = 0,
    IE_KEY_PRESSED = 1,
    IE_KEY_HELD = 2,
    IE_KEY_RELEASED = 3,
    IE_LMB_PRESSED = 4,
    IE_LMB_HELD = 5,
    IE_LMB_RELEASED = 6,
    IE_MMB_PRESSED = 7,
    IE_MMB_HELD = 8,
    IE_MMB_RELEASED = 9,
    IE_RMB_PRESSED = 10,
    IE_RMB_HELD = 11,
    IE_RMB_RELEASED = 12,
    IE_MOUSE_MOVEMENT = 13,
    IE_MOUSE_WHEEL_MOVEMENT = 14,
    IE_CHAR_PRESSED = 15,
    IE_CHAR_RELEASED = 16,
    IE_JOYSTICK_POSITION = 17,
    IE_JOYSTICK_POSITION2 = 18,
    IE_JOYSTICK_BUTTON_PRESSED = 19,
    IE_JOYSTICK_BUTTON_HELD = 20,
    IE_JOYSTICK_BUTTON_RELEASED = 21,
    IE_MB4_PRESSED = 22,
    IE_MB5_PRESSED = 23,
    IE_MB6_PRESSED = 24,
    IE_MB7_PRESSED = 25,
    IE_MB8_PRESSED = 26,
    IE_MB4_HELD = 27,
    IE_MB5_HELD = 28,
    IE_MB6_HELD = 29,
    IE_MB7_HELD = 30,
    IE_MB8_HELD = 31,
    IE_MB4_RELEASED = 32,
    IE_MB5_RELEASED = 33,
    IE_MB6_RELEASED = 34,
    IE_MB7_RELEASED = 35,
    IE_MB8_RELEASED = 36
};

// FableWin.pdb enum EInputDeviceType.
enum EInputDeviceType
{
    INPUT_DEVICE_TYPE_NULL = 0,
    INPUT_DEVICE_TYPE_JOYSTICK = 1,
    INPUT_DEVICE_TYPE_KEYBOARD = 2,
    INPUT_DEVICE_TYPE_MOUSE = 3
};

// FableWin.pdb enum EXboxControllerButton.
enum EXboxControllerButton
{
    XBOX_PAD_UNDEFINED_BUTTON = 0,
    XBOX_PAD_X_BUTTON = 1,
    XBOX_PAD_Y_BUTTON = 2,
    XBOX_PAD_BLACK_BUTTON = 3,
    XBOX_PAD_A_BUTTON = 4,
    XBOX_PAD_B_BUTTON = 5,
    XBOX_PAD_WHITE_BUTTON = 6,
    XBOX_PAD_LEFT_TRIGGER = 7,
    XBOX_PAD_RIGHT_TRIGGER = 8,
    XBOX_PAD_LEFT_STICK_BUTTON = 9,
    XBOX_PAD_RIGHT_STICK_BUTTON = 10,
    XBOX_PAD_START_BUTTON = 11,
    XBOX_PAD_BACK_BUTTON = 12,
    XBOX_PAD_DPAD_UP_BUTTON = 13,
    XBOX_PAD_DPAD_DOWN_BUTTON = 14,
    XBOX_PAD_DPAD_LEFT_BUTTON = 15,
    XBOX_PAD_DPAD_RIGHT_BUTTON = 16,
    XBOX_PAD_LEFT_ANALOGUE_STICK = 17,
    XBOX_PAD_RIGHT_ANALOGUE_STICK = 18
};

// Ego_r.pdb: { bool Started; long Index; } (8 bytes). Retail callers clear
// Started only.
class CInputEventScanInfo
{
public:
    CInputEventScanInfo() : Started(false) {}

    bool Started;
    long Index;
};

FABLE_STATIC_ASSERT(sizeof(CInputEventScanInfo) == 0x08);

// Retail 0x34 bytes. Ego_r.pdb UInputEventData (0x20): a union at +0x00
// (key / char / JoystickPos + JoystickButton at +0x08) and a union at +0x0C
// (MouseMovement C3DVector, MousePos C2DVector at +0x18; the mouse-wheel
// float at +0x14). Retail SetAsMouseMovement 0x00A03FB0 writes movement to
// +0x0C and the position to +0x18.
class CInputEvent
{
public:
    CInputEvent();                                   // 0x00A04410
    ~CInputEvent();                                  // 0x00A03B30 (empty)

    EInputEventType GetType() const;                 // 0x00A03B40
    EInputDeviceType GetDeviceType() const;          // 0x00A03B50
    long GetDeviceNumber() const;                    // 0x00A03B60
    EInputKey GetKey() const;                        // 0x00A03B70
    wchar_t GetChar() const;                         // 0x00A03B80
    long GetJoystickButton() const;                  // 0x00A03B90
    float GetJoystickButtonPressureFraction() const; // 0x00A03BA0
    float GetMouseWheelMovement() const;             // 0x00A03BB0
    C2DVector GetMousePos() const;                   // 0x00A04380
    C2DVector GetJoystickPosition() const;           // 0x00A043A0

    // Inlined by retail CNewFrontendGameComponent::Input (reads +0x0C/+0x10).
    // Ego_r ?GetMouseMovement@CInputEvent@@QBE?AVC3DVector@@XZ: out of line at
    // 0x00A043D0 (CInputEvent_Getters_00a043d0.cpp, which defines
    // FABLE_INPUT_EVENT_MOVEMENT_OUT_OF_LINE), inline for other callers.
    C3DVector GetMouseMovement() const;
    // Ego_r ?GetHeldMouseMovement@CInputEvent@@QBE?AVC3DVector@@XZ, retail 0x00A043F0.
    C3DVector GetHeldMouseMovement() const;

    // Ego_r lib_input_event.cpp setters, retail 0x00A03BC0-0x00A0415F in Ego_r
    // order (CInputEvent_Setters_00a03bc0.cpp).
    void SetMousePos(const C2DVector& pos);
    void SetAsNull();
    void SetAsKeyboardKeyPressed(EInputKey key, double time);
    void SetAsKeyboardKeyHeld(EInputKey key, double start_time, double event_time);
    void SetAsKeyboardKeyReleased(EInputKey key, double start_time, double event_time);
    void SetAsLeftMouseButtonPressed(const C2DVector& pos, double time);
    void SetAsCharPressed(wchar_t tchar, double time);
    void SetAsCharReleased(wchar_t tchar, double time);
    void SetAsLeftMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double start_time, double current_time);
    void SetAsLeftMouseButtonReleased(const C2DVector& pos, double start_time, double current_time);
    void SetAsMiddleMouseButtonPressed(const C2DVector& pos, double time);
    void SetAsMiddleMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double start_time, double current_time);
    void SetAsMiddleMouseButtonReleased(const C2DVector& pos, double start_time, double current_time);
    void SetAsRightMouseButtonPressed(const C2DVector& pos, double time);
    void SetAsRightMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double start_time, double current_time);
    void SetAsRightMouseButtonReleased(const C2DVector& pos, double start_time, double current_time);
    void SetAsExtraMouseButtonPressed(const C2DVector& pos, double time, long x);
    void SetAsExtraMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double start_time, double current_time, long x);
    void SetAsExtraMouseButtonReleased(const C2DVector& pos, double start_time, double current_time, long x);
    void SetAsMouseMovement(const C2DVector& pos, const C3DVector& movement, double time);
    void SetAsMouseWheelMovement(float move, double time);
    void SetAsJoystickPosition(const C2DVector& pos, double time);
    void SetAsJoystickPosition2(const C2DVector& pos, double time);
    void SetAsJoystickButtonPressed(long button, float button_pressure_fraction, const C2DVector& pos, double time);
    void SetAsJoystickButtonHeld(long button, float button_pressure_fraction, const C2DVector& pos, double time);
    void SetAsJoystickButtonReleased(long button, float button_pressure_fraction, const C2DVector& pos, double time);
    void SetJoystickButtonPressure(float button_pressure_fraction);

private:
    // UInputEventData first union (FableWin.pdb names; retail getters
    // 0x00A03B70 / 0x00A03B80 / 0x00A03B90 read +0x00 dword / word, +0x08 byte).
    union
    {
        EInputKey Key;                               // +0x00
        wchar_t Char;                                // +0x00
        struct
        {
            float JoystickPosStorage_[2];            // +0x00 C2DVector JoystickPos
            unsigned char JoystickButton;            // +0x08
            fable_u8 Data09_[3];
        };
        float HeldMouseMovementStorage_[3];          // +0x00 C3DVector (mouse-button held events)
    };
    // UInputEventData second union: MouseMovement (C3DVector; .Z = mouse wheel,
    // 0x00A03BB0) or the joystick button pressure fraction.
    union
    {
        float MouseMovementStorage_[3];              // +0x0C C3DVector MouseMovement
        float JoystickButtonPressureFraction;        // +0x0C
    };
    C2DVector MousePos;                              // +0x18

    C3DVector& MouseMovementRef() { return reinterpret_cast<C3DVector&>(MouseMovementStorage_); }
    C3DVector& HeldMouseMovementRef() { return reinterpret_cast<C3DVector&>(HeldMouseMovementStorage_); }
    EInputDeviceType DeviceType;                     // +0x20
    long DeviceNumber;                               // +0x24
    EInputEventType Type;                            // +0x28
    float CurrentTime;                               // +0x2C
    float StartTime;                                 // +0x30
};

FABLE_STATIC_ASSERT(sizeof(CInputEvent) == 0x34);

#ifndef FABLE_INPUT_EVENT_MOVEMENT_OUT_OF_LINE
inline C3DVector CInputEvent::GetMouseMovement() const
{
    return reinterpret_cast<const C3DVector&>(MouseMovementStorage_);
}
#endif

// Retail vtable 0x0129D620 (CInputManagerDX): scalar deleting destructor,
// SetExclusiveMode 0x009F4E70, Update 0x009F57A0.
// ?GPInputManager@@3PAVCInputManager@@A is retail 0x013B8388 (FableWin puts
// GPDisplayManager 8 bytes after it, as retail's 0x013B8390 is).
class CInputManager;
extern CInputManager* g_FableInputManager_013B8388;

// ?Get@CGameJoystickManager@@SIAAV1@XZ singleton (retail 0x0049BB90; its
// PClass slot is 0x013B89A0). Ego_r.pdb layout: vptr,
// MainPlayerJoystickDeviceNumber +4, MainPlayerJoystickDeviceNumberSet +8.
// RTTI: CGameJoystickManager : CBaseClassNonCopyable (vtable 0x01238B54; the
// base ctor 0x0099A2F0 is out of line, the class ctor inline in Get).
class CGameJoystickManager : public CBaseClassNonCopyable
{
public:
    // ??0CGameJoystickManager@@QAE@XZ; inline in retail Get 0x0049BB90.
    CGameJoystickManager()
        : MainPlayerJoystickDeviceNumber(0),
          MainPlayerJoystickDeviceNumberSet(false)
    {
    }
    virtual ~CGameJoystickManager();                            // ??1CGameJoystickManager@@UAE@XZ

    static CGameJoystickManager& FABLE_FASTCALL Get();          // 0x0049BB90
    bool HasExplicitMainPlayerJoystickDeviceNumber() const;     // 0x0049BB50
    long GetMainPlayerJoystickDeviceNumber() const;             // 0x0049BB40

private:
    // ?PClass@CGameJoystickManager@@0V?$CCountedPointer@VCGameJoystickManager@@@@A
    // (retail 0x013B89A0).
    static CCountedPointer<CGameJoystickManager> PClass;

    long MainPlayerJoystickDeviceNumber;                        // +0x04
    bool MainPlayerJoystickDeviceNumberSet;                     // +0x08
};

FABLE_STATIC_ASSERT(sizeof(CGameJoystickManager) == 0x0C);
