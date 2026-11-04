// CKeyboardDX::GetKeyName (retail 0x00AB6180; the unit is built with /GS - the property buffer holds a string): the
// keyboard layout's name for the DirectInput key a game key maps to.
#include "fable_dinput8.h"
#include "fable_keyboard_dx.h"
#include "fable_string.h"

bool CKeyboardDX::GetKeyName(EInputKey key, CWideString& name) const
{
    DIPROPSTRING property;
    property.diph.dwSize = sizeof(DIPROPSTRING);
    property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    property.diph.dwObj = InputKeyToDXKey[key];
    property.diph.dwHow = DIPH_BYOFFSET;
    if (SUCCEEDED(DKeyboard->GetProperty(DIPROP_KEYNAME, &property.diph)))
    {
        name = property.wsz;
        return true;
    }
    return false;
}
