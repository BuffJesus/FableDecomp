#include <stdio.h>
#include <string.h>

#include "fable_registry.h"

namespace
{
    fable_u32 g_setCalls = 0;
    void* g_key = 0;
    const wchar_t* g_name = 0;
    fable_u32 g_type = 0;
    fable_u32 g_size = 0;
    int g_value = -1;

    fable_i32 FABLE_STDCALL FakeSet(
        void* key,
        const wchar_t* valueName,
        fable_u32 reserved,
        fable_u32 type,
        const fable_u8* data,
        fable_u32 dataSize)
    {
        ++g_setCalls;
        g_key = key;
        g_name = valueName;
        g_type = type;
        g_size = dataSize;
        if (reserved != 0 || data == 0 || dataSize != sizeof(int))
            return -1;
        g_value = *reinterpret_cast<const int*>(data);
        return 0;
    }

    struct RegistryView
    {
        void* rootKey;
        void* openKey;
    };
}

FableRegSetValueExWFunction
    g_FableRegSetValueExW_0143FBE8 = FakeSet;

CWideString::operator const wchar_t*() const
{
    const CWideStringData* storage =
        *reinterpret_cast<CWideStringData* const*>(this);
    return storage != 0 ? storage->text : L"";
}

int main()
{
    wchar_t valueText[] = L"FirstRun";
    CWideStringData wideStorage = {};
    wideStorage.text = valueText;
    CWideStringData* wideSlot = &wideStorage;
    CWideString& valueName =
        *reinterpret_cast<CWideString*>(&wideSlot);

    unsigned char registryStorage[sizeof(CSystemRegistry)];
    RegistryView& view =
        *reinterpret_cast<RegistryView*>(registryStorage);
    CSystemRegistry& registry =
        *reinterpret_cast<CSystemRegistry*>(registryStorage);
    view.rootKey = 0;
    view.openKey = 0;

    registry.WriteBOOL(valueName, 1);
    if (g_setCalls != 0)
        return 1;

    view.openKey = reinterpret_cast<void*>(0x12345678UL);
    registry.WriteBOOL(valueName, 0);
    if (
        g_setCalls != 1 ||
        g_key != view.openKey ||
        wcscmp(g_name, valueText) != 0 ||
        g_type != 4 ||
        g_size != 4 ||
        g_value != 0)
    {
        return 2;
    }

    puts("FABLETLC_SYSTEM_REGISTRY_WRITE_BOOL_BEHAVIOR PASS");
    return 0;
}
