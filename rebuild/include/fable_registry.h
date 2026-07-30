#pragma once

#include "fable_string.h"

typedef fable_i32 (FABLE_STDCALL *FableRegCreateKeyExWFunction)(
    void* rootKey,
    const wchar_t* subKey,
    fable_u32 reserved,
    wchar_t* keyClass,
    fable_u32 options,
    fable_u32 access,
    void* securityAttributes,
    void** resultKey,
    fable_u32* disposition);

typedef fable_i32 (FABLE_STDCALL *FableRegSetValueExWFunction)(
    void* key,
    const wchar_t* valueName,
    fable_u32 reserved,
    fable_u32 type,
    const fable_u8* data,
    fable_u32 dataSize);

typedef fable_i32 (FABLE_STDCALL *FableRegCloseKeyFunction)(
    void* key);

extern FableRegCreateKeyExWFunction
    g_FableRegCreateKeyExW_0143FBF8;
extern FableRegSetValueExWFunction
    g_FableRegSetValueExW_0143FBE8;
extern FableRegCloseKeyFunction
    g_FableRegCloseKey_0143FBFC;

class CSystemRegistry
{
public:
    CSystemRegistry(
        const CWideString& subKey,
        long rootSelection);
    ~CSystemRegistry();

    void WriteBOOL(
        const CWideString& valueName,
        int value) const;

private:
    void* rootKey_;
    void* openKey_;
};

FABLE_STATIC_ASSERT(sizeof(CSystemRegistry) == 0x08);
