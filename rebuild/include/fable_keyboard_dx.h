#pragma once
// Hand-owned (retail model): CKeyboardDX, the DirectInput keyboard. Ego_r.pdb names, members and access; retail RTTI
// vtable 0x0129EA60: [0] ??_E 0x00AB6A50, [1] Update 0x00AB69F0, [2] SetExclusiveMode 0x00AB6450, [3] SetHasInputFocus
// 0x00AB6060, [4] GetKeyName 0x00AB6180, and its own [5] IsKeyPressed 0x00AB6220 (CKeyboard 0x0129DBA8 has five
// slots). Ego_r CKeyboardDX is 0x361C over a 0x341C CKeyboard; retail's STLport list is one pointer, so CKeyboard is
// 0x3414 and every member below sits 8 bytes lower (retail 0x3614).
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_input_devices.h"

struct IDirectInputDevice8W;
class CInputManagerDX;

class CKeyboardDX : public CKeyboard
{
public:
    virtual ~CKeyboardDX();
    virtual bool Update();
    virtual bool SetExclusiveMode(bool exclusive);
    virtual void SetHasInputFocus(bool hasFocus);
    virtual bool GetKeyName(EInputKey key, CWideString& name) const;
    // Whether the DirectInput key the game key maps to is down in the last polled state.
    virtual bool IsKeyPressed(EInputKey key);

protected:
    friend struct CKeyboardDXLayoutCheck;
    CInputManagerDX* InputManagerDX;                 // +0x3414 (Ego_r: CInputManagerDX&)
    IDirectInputDevice8W* DKeyboard;                 // +0x3418
    signed char KeyStates[256];                      // +0x341C DirectInput key state bytes (Ego_r: char[256])
    unsigned short InputKeyToDXKey[122];             // +0x351C EInputKey -> DIK_* scan code
    bool Acquired;                                   // +0x3610
};

struct CKeyboardDXLayoutCheck
{
    FABLE_STATIC_ASSERT(offsetof(CKeyboardDX, InputManagerDX) == 0x3414);
    FABLE_STATIC_ASSERT(offsetof(CKeyboardDX, KeyStates) == 0x341C);
    FABLE_STATIC_ASSERT(offsetof(CKeyboardDX, InputKeyToDXKey) == 0x351C);
    FABLE_STATIC_ASSERT(offsetof(CKeyboardDX, Acquired) == 0x3610);
};
FABLE_STATIC_ASSERT(sizeof(CKeyboardDX) == 0x3614);
