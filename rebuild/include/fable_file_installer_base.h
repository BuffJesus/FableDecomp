#pragma once
#include "fable_singleton.h"

class CFileInstaller;
// Shipping RTTI and deleting destructor 009C5B60 establish this polymorphic
// specialization. The generic singleton used by other classes is unchanged.
template<> class CTBaseSingleton<CFileInstaller>
{
public:
    CTBaseSingleton() {}
    virtual ~CTBaseSingleton() {}
    static CFileInstaller& FABLE_FASTCALL Get();
    static void FABLE_FASTCALL Release();
};
FABLE_STATIC_ASSERT(sizeof(CTBaseSingleton<CFileInstaller>) == 4);
