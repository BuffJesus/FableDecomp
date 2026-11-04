#include <new>
#include <string.h>
#include "fable_registry.h"

// The advapi32 imports, called directly (retail caches the import address in
// a register when a function calls one twice).
extern "C" __declspec(dllimport) long __stdcall RegQueryValueExW(void* key, const wchar_t* valueName,
    fable_u32* reserved, fable_u32* type, fable_u8* data, fable_u32* dataSize);
extern "C" __declspec(dllimport) long __stdcall RegSetValueExW(void* key, const wchar_t* valueName,
    fable_u32 reserved, fable_u32 type, const fable_u8* data, fable_u32 dataSize);
extern "C" __declspec(dllimport) long __stdcall RegDeleteValueW(void* key, const wchar_t* valueName);

// lib_system_registry.cpp value accessors (Ego_r CSystemRegistry members;
// GetString's Buffer is wchar_t[512] with a 512 byte size, so the unit
// builds with /GS). Retail 0099CF80..0099D1B0. GetBOOL 0099CFE0 replaces a
// fake-struct landing (CEngineMap::PeekWaterHasType).

// Retail 0099CF80: a REG_DWORD, or the default.
long CSystemRegistry::GetSLONG(const CWideString& valueName, long defaultValue) const
{
    if (openKey_ != 0)
    {
        fable_u32 type = 0;
        long value = 0;
        fable_u32 size = 4;
        if (RegQueryValueExW(openKey_, valueName.GetAsCharArray(), 0, &type,
                                             reinterpret_cast<fable_u8*>(&value), &size) == 0)
            return value;
    }
    return defaultValue;
}

// Retail 0099CFE0.
int CSystemRegistry::GetBOOL(const CWideString& valueName, int defaultValue) const
{
    bool value = GetSLONG(valueName, defaultValue) != 0;
    return value;
}

// Retail 0099D000.
CWideString CSystemRegistry::GetString(const CWideString& valueName, const CWideString& defaultValue) const
{
    wchar_t buffer[512];
    if (openKey_ != 0)
    {
        fable_u32 type = 0;
        fable_u32 size = 512;
        if (RegQueryValueExW(openKey_, valueName.GetAsCharArray(), 0, &type,
                                             reinterpret_cast<fable_u8*>(buffer), &size) == 0)
            return CWideString(buffer);
    }
    return defaultValue;
}

// Retail 0099D0C0.
void CSystemRegistry::WriteSLONG(const CWideString& valueName, long value) const
{
    if (openKey_ != 0)
    {
        long storedValue = value;
        RegSetValueExW(openKey_, valueName.GetAsCharArray(), 0, 4,
                                       reinterpret_cast<const fable_u8*>(&storedValue), 4);
    }
}

// Retail 0099D140: REG_SZ; the size is the character count plus one.
void CSystemRegistry::WriteString(const CWideString& valueName, const CWideString& value) const
{
    if (openKey_ != 0)
        RegSetValueExW(openKey_, valueName.GetAsCharArray(), 0, 1,
                                       reinterpret_cast<const fable_u8*>(value.GetAsCharArray()),
                                       value.GetLength() + 1);
}

// Retail 0099D180: REG_BINARY.
void CSystemRegistry::WriteBinary(const CWideString& valueName, void* value, unsigned long size) const
{
    if (openKey_ != 0)
        RegSetValueExW(openKey_, valueName.GetAsCharArray(), 0, 3,
                                       static_cast<const fable_u8*>(value), size);
}

// Retail 0099D1B0: the value's size (no open-key test).
unsigned long CSystemRegistry::GetSize(const CWideString& valueName) const
{
    fable_u32 size;
    RegQueryValueExW(openKey_, valueName.GetAsCharArray(), 0, 0, 0, &size);
    return size;
}

// Retail 0099D1E0.
void CSystemRegistry::DeleteValue(const CWideString& valueName) const
{
    RegDeleteValueW(openKey_, valueName.GetAsCharArray());
}

// Retail 0099D200: read the whole value, then copy `size` bytes of it,
// zero-filling past a shorter stored value.
void CSystemRegistry::GetBinary(const CWideString& valueName, void* data, unsigned long size) const
{
    if (openKey_ != 0)
    {
        fable_u32 whole_size;
        RegQueryValueExW(openKey_, valueName.GetAsCharArray(), 0, 0, 0, &whole_size);
        fable_u32 read_size = whole_size;
        fable_u8* buffer = new fable_u8[whole_size];
        RegQueryValueExW(openKey_, valueName.GetAsCharArray(), 0, 0, buffer, &read_size);
        if (read_size >= size)
        {
            memcpy(data, buffer, size);
        }
        else
        {
            memset(data, 0, size);
            memcpy(data, buffer, read_size);
        }
        delete[] buffer;
    }
}
