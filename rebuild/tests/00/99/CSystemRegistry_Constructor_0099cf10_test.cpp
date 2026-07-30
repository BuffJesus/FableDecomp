#include <stdio.h>
#include <string.h>

#include "fable_registry.h"

namespace
{
    fable_u32 g_calls = 0;
    void* g_root = 0;
    const wchar_t* g_subKey = 0;
    fable_i32 g_result = 0;
    void* g_createdKey = reinterpret_cast<void*>(0x12345678UL);

    fable_i32 FABLE_STDCALL FakeCreate(
        void* rootKey,
        const wchar_t* subKey,
        fable_u32 reserved,
        wchar_t* keyClass,
        fable_u32 options,
        fable_u32 access,
        void* securityAttributes,
        void** resultKey,
        fable_u32* disposition)
    {
        ++g_calls;
        g_root = rootKey;
        g_subKey = subKey;
        if (
            reserved != 0 ||
            keyClass != 0 ||
            options != 0 ||
            access != 0x0002001FUL ||
            securityAttributes != 0 ||
            disposition != 0)
        {
            return -1;
        }
        *resultKey = g_createdKey;
        return g_result;
    }

    struct RegistryView
    {
        void* rootKey;
        void* openKey;
    };

    CWideString& MakeWide(
        CWideStringData*& slot,
        CWideStringData& storage,
        wchar_t* text)
    {
        memset(&storage, 0, sizeof(storage));
        storage.text = text;
        slot = &storage;
        return *reinterpret_cast<CWideString*>(&slot);
    }
}

FableRegCreateKeyExWFunction
    g_FableRegCreateKeyExW_0143FBF8 = FakeCreate;

CSystemRegistry::~CSystemRegistry()
{
}

CWideString::operator const wchar_t*() const
{
    const CWideStringData* storage =
        *reinterpret_cast<CWideStringData* const*>(this);
    return storage != 0 ? storage->text : L"";
}

int main()
{
    wchar_t keyText[] = L"Software\\Lionhead Studios Ltd\\Fable";
    CWideStringData storage;
    CWideStringData* slot = 0;
    CWideString& key = MakeWide(slot, storage, keyText);

    CSystemRegistry currentUser(key, 0);
    RegistryView& currentUserView =
        reinterpret_cast<RegistryView&>(currentUser);
    if (
        g_calls != 1 ||
        g_root != reinterpret_cast<void*>(0x80000001UL) ||
        wcscmp(g_subKey, keyText) != 0 ||
        currentUserView.openKey != g_createdKey)
    {
        return 1;
    }

    CSystemRegistry classes(key, 2);
    if (g_root != reinterpret_cast<void*>(0x80000000UL))
        return 2;

    CSystemRegistry localMachine(key, 7);
    if (g_root != reinterpret_cast<void*>(0x80000002UL))
        return 3;

    g_result = 5;
    CSystemRegistry failed(key, 0);
    RegistryView& failedView =
        reinterpret_cast<RegistryView&>(failed);
    if (failedView.openKey != 0)
        return 4;

    puts("FABLETLC_SYSTEM_REGISTRY_CONSTRUCTOR_BEHAVIOR PASS");
    return 0;
}
