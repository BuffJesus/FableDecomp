// ABI probe, never linked: defining each device's key function (its
// destructor) emits the base vtable for comparison with retail CMouse
// 0x0129DBC4, CKeyboard 0x0129DBA8 and CJoystick 0x0129C0EC.
#include "fable_input_devices.h"
CMouse::~CMouse() {}
CKeyboard::~CKeyboard() {}
CJoystick::~CJoystick() {}
