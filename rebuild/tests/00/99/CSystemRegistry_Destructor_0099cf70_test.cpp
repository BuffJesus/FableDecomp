#include <stdio.h>

#include "fable_registry.h"

namespace
{
    fable_u32 g_closeCalls = 0;
    void* g_closedKey = 0;

    fable_i32 FABLE_STDCALL FakeClose(void* key)
    {
        ++g_closeCalls;
        g_closedKey = key;
        return 0;
    }

    struct RegistryView
    {
        void* rootKey;
        void* openKey;
    };
}

FableRegCloseKeyFunction
    g_FableRegCloseKey_0143FBFC = FakeClose;

int main()
{
    unsigned char storage[sizeof(CSystemRegistry)];
    RegistryView& view = *reinterpret_cast<RegistryView*>(storage);
    CSystemRegistry& registry =
        *reinterpret_cast<CSystemRegistry*>(storage);

    view.rootKey = 0;
    view.openKey = 0;
    registry.~CSystemRegistry();
    if (g_closeCalls != 0)
        return 1;

    view.openKey = reinterpret_cast<void*>(0x12345678UL);
    registry.~CSystemRegistry();
    if (
        g_closeCalls != 1 ||
        g_closedKey != reinterpret_cast<void*>(0x12345678UL))
    {
        return 2;
    }

    puts("FABLETLC_SYSTEM_REGISTRY_DESTRUCTOR_BEHAVIOR PASS");
    return 0;
}
