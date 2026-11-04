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
    // Ego_r CSystemRegistry value accessors (parameters Name, Default, Value,
    // size): GetSLONG 0x0099CF80, GetBOOL 0x0099CFE0, GetString 0x0099D000,
    // WriteSLONG 0x0099D0C0, WriteString 0x0099D140, WriteBinary 0x0099D180,
    // GetSize 0x0099D1B0.
    long GetSLONG(const CWideString& valueName, long defaultValue) const;
    int GetBOOL(
        const CWideString& valueName,
        int defaultValue) const;
    CWideString GetString(const CWideString& valueName, const CWideString& defaultValue) const;
    void WriteSLONG(const CWideString& valueName, long value) const;
    void WriteString(const CWideString& valueName, const CWideString& value) const;
    void WriteBinary(const CWideString& valueName, void* value, unsigned long size) const;
    unsigned long GetSize(const CWideString& valueName) const;
    // DeleteValue 0x0099D1E0, GetBinary 0x0099D200 (Ego_r local whole_size).
    void DeleteValue(const CWideString& valueName) const;
    void GetBinary(const CWideString& valueName, void* data, unsigned long size) const;

    // Retail 0x013BCA28 / 0x013BCA2C (FableWin.pdb static members).
    static const CWideString GAME_REG_KEY;
    static const CWideString GFX_RESET_KEY_NAME;

private:
    void* rootKey_;
    void* openKey_;
};

FABLE_STATIC_ASSERT(sizeof(CSystemRegistry) == 0x08);
