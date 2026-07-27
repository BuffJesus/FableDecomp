#include "fable_string.h"

#include <stdio.h>
#include <wchar.h>

namespace
{
    wchar_t g_leftBuffer[64];
    wchar_t g_resultBuffer[64];
    CWideStringData g_leftStorage;
    CWideStringData g_resultStorage;
    unsigned int g_copyCalls;
    unsigned int g_appendLiteralCalls;

    CWideStringData*& Storage(CWideString& text)
    {
        return *reinterpret_cast<CWideStringData**>(&text);
    }
}

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;

extern "C" const CWideString& __fastcall
FableWideStringAppendLiteral_0099B940(
    CWideString* self,
    void*,
    const wchar_t* text)
{
    return self->operator+=(text);
}

extern "C" void __fastcall
FableWideStringUnassign_0099B4D0(CWideString* self)
{
    Storage(*self) = 0;
}

extern "C" void __cdecl FableWideStringFree_00BFEA14(void*)
{
}

extern "C" void __cdecl FableWideStringDelete_00BFE9BC(void*)
{
}

const CWideString& CWideString::operator+=(const wchar_t* text)
{
    ++g_appendLiteralCalls;
    ++g_copyCalls;
    g_resultStorage = *Storage(*this);
    wcscpy(g_resultBuffer, g_resultStorage.text);
    g_resultStorage.text = g_resultBuffer;
    g_resultStorage.owners = 8;
    Storage(*this) = &g_resultStorage;
    wcscat(Storage(*this)->text, text);
    return *this;
}

int main()
{
    wcscpy(g_leftBuffer, L"Data\\lang\\English");
    g_leftStorage.text = g_leftBuffer;
    g_leftStorage.unknown04 = 0;
    g_leftStorage.unknown08 = 0;
    g_leftStorage.owners = 8;

    CWideString left;
    Storage(left) = &g_leftStorage;
    CWideString result = left + L"\\lang_settings.txt";

    if (
        wcscmp(
            static_cast<const wchar_t*>(result),
            L"Data\\lang\\English\\lang_settings.txt") != 0 ||
        wcscmp(g_leftBuffer, L"Data\\lang\\English") != 0 ||
        g_copyCalls != 1 ||
        g_appendLiteralCalls != 1 ||
        g_resultStorage.owners != 8 ||
        g_CWideStringInstanceCount_013BCA20 != 1)
    {
        return 1;
    }

    printf("FABLETLC_WIDE_STRING_ADD_RIGHT PASS\n");
    return 0;
}

CWideString::CWideString()
{
    Storage(*this) = 0;
}

CWideString::~CWideString()
{
    Storage(*this) = 0;
}

CWideString::operator const wchar_t*() const
{
    return Storage(const_cast<CWideString&>(*this))->text;
}
