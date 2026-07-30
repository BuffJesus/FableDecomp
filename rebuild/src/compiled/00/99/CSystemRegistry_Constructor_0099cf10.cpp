#include "fable_registry.h"

CSystemRegistry::CSystemRegistry(
    const CWideString& subKey,
    long rootSelection)
{
    switch (rootSelection)
    {
    case 0:
        rootKey_ = reinterpret_cast<void*>(0x80000001UL);
        break;
    case 2:
        rootKey_ = reinterpret_cast<void*>(0x80000000UL);
        break;
    default:
        rootKey_ = reinterpret_cast<void*>(0x80000002UL);
        break;
    }

    const fable_i32 result =
        g_FableRegCreateKeyExW_0143FBF8(
            rootKey_,
            static_cast<const wchar_t*>(subKey),
            0,
            0,
            0,
            0x0002001FUL,
            0,
            &openKey_,
            0);
    if (result != 0)
        openKey_ = 0;
}
