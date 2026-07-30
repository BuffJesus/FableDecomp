#include "fable_registry.h"

void CSystemRegistry::WriteBOOL(
    const CWideString& valueName,
    int value) const
{
    if (openKey_ != 0)
    {
        int storedValue = value;
        g_FableRegSetValueExW_0143FBE8(
            openKey_,
            static_cast<const wchar_t*>(valueName),
            0,
            4,
            reinterpret_cast<const fable_u8*>(&storedValue),
            4);
    }
}
