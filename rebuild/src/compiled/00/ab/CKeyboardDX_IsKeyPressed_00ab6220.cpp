// CKeyboardDX::IsKeyPressed (retail 0x00AB6220): a game key is down when its DirectInput key's state byte is set.
#include "fable_keyboard_dx.h"

bool CKeyboardDX::IsKeyPressed(EInputKey key)
{
    return KeyStates[InputKeyToDXKey[key]] ? true : false;   // `!= 0` zeroes eax first; retail tests the byte in al
}
