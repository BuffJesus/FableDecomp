#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fable_language.h"
#include "fable_resource_directories.h"

namespace
{
    const wchar_t* WideText(const CWideString& value)
    {
        return *reinterpret_cast<const wchar_t* const*>(&value);
    }
}

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;
fable_i32 g_CCharStringInstanceCount_013BD800 = 0;

CWideString::CWideString(const wchar_t* text)
{
    storage_ = reinterpret_cast<CWideStringData*>(
        const_cast<wchar_t*>(text));
}

CWideString::~CWideString()
{
}

CCharString::CCharString(const char* text, long /* length */)
{
    storage_ = reinterpret_cast<CCharStringData*>(
        const_cast<char*>(text));
}

CCharString::~CCharString()
{
}

CWideString CCharString::ToWideString() const
{
    return CWideString(L"English");
}

CCharString FABLE_FASTCALL
FableGetDefaultLanguageName_00415530()
{
    return CCharString("English", -1);
}

CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right)
{
    static wchar_t combined[64];
    wcscpy(combined, left);
    wcscat(combined, WideText(right));
    return CWideString(combined);
}

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right)
{
    static wchar_t combined[64];
    wcscpy(combined, WideText(left));
    wcscat(combined, right);
    return CWideString(combined);
}

int main()
{
    CWideString value = FABLE_LANGUAGE_DIRECTORY_FUNCTION();
    if (wcscmp(WideText(value), L"Data\\lang\\English\\") != 0)
    {
        printf(FABLE_LANGUAGE_DIRECTORY_FAIL "\n");
        return 1;
    }

    printf(FABLE_LANGUAGE_DIRECTORY_PASS "\n");
    return 0;
}
