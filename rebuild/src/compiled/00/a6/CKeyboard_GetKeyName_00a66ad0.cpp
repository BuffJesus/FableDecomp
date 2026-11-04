#include "fable_input_devices.h"
// The base keyboard has no key names.
bool CKeyboard::GetKeyName(EInputKey, CWideString&) const
{
    return false;
}
