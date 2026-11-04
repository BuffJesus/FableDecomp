// CKeyboardDX::SetHasInputFocus (retail 0x00AB6060): losing focus releases the DirectInput keyboard; gaining it
// re-acquires the device and, when it was not held before, throws away whatever DirectInput buffered meanwhile so no
// stale key presses arrive.
#include "fable_dinput8.h"
#include "fable_keyboard_dx.h"

void CKeyboardDX::SetHasInputFocus(bool hasFocus)
{
    if (!hasFocus)
    {
        if (Acquired == true)
        {
            const HRESULT result = DKeyboard->Unacquire();
            if (result == DI_OK || result == DI_NOEFFECT)
                Acquired = false;
        }
    }
    else
    {
        const HRESULT acquired = DKeyboard->Acquire();
        const bool wasAcquired = Acquired;               // read after the call, as retail does
        Acquired = SUCCEEDED(acquired);
        if (!wasAcquired)
        {
            DIDEVICEOBJECTDATA buffered[256];
            DWORD count = 256;
            while (count != 0 && SUCCEEDED(DKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), buffered, &count, 0)))
            {
            }
        }
    }
}
