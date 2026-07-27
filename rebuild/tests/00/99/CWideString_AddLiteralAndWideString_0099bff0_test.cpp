#include "fable_string.h"

#include <stdio.h>
#include <wchar.h>

namespace
{
    wchar_t g_resultBuffer[64];
    wchar_t g_rightBuffer[32];
    CWideStringData g_resultStorage;
    CWideStringData g_rightStorage;
    unsigned int g_literalConstructorCalls;
    unsigned int g_appendWideCalls;

    CWideStringData*& Storage(CWideString& text)
    {
        return *reinterpret_cast<CWideStringData**>(&text);
    }
}

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;

extern "C" void __fastcall
FableWideStringConstructLiteral_0099B6B0(
    CWideString* self,
    void*,
    const wchar_t* text)
{
    ++g_literalConstructorCalls;
    ++g_CWideStringInstanceCount_013BCA20;
    wcscpy(g_resultBuffer, text);
    g_resultStorage.text = g_resultBuffer;
    g_resultStorage.unknown04 = 0;
    g_resultStorage.unknown08 = 0;
    g_resultStorage.owners = 8;
    Storage(*self) = &g_resultStorage;
}

extern "C" const CWideString& __fastcall
FableWideStringAppendWide_0099B8D0(
    CWideString* self,
    void*,
    const CWideString& text)
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

CWideString::CWideString(const wchar_t* text)
{
    ++g_literalConstructorCalls;
    wcscpy(g_resultBuffer, text);
    g_resultStorage.text = g_resultBuffer;
    g_resultStorage.unknown04 = 0;
    g_resultStorage.unknown08 = 0;
    g_resultStorage.owners = 8;
    Storage(*this) = &g_resultStorage;
}

const CWideString& CWideString::operator+=(const CWideString& other)
{
    ++g_appendWideCalls;
    wcscat(Storage(*this)->text, Storage(const_cast<CWideString&>(other))->text);
    return *this;
}

int main()
{
    wcscpy(g_rightBuffer, L"English");
    g_rightStorage.text = g_rightBuffer;
    g_rightStorage.unknown04 = 0;
    g_rightStorage.unknown08 = 0;
    g_rightStorage.owners = 8;

    CWideString right;
    Storage(right) = &g_rightStorage;
    CWideString result = L"Data\\lang\\" + right;

    if (
        wcscmp(
            static_cast<const wchar_t*>(result),
            L"Data\\lang\\English") != 0 ||
        wcscmp(g_rightBuffer, L"English") != 0 ||
        g_literalConstructorCalls != 1 ||
        g_appendWideCalls != 1 ||
        g_resultStorage.owners != 8 ||
        g_CWideStringInstanceCount_013BCA20 != 1)
    {
        return 1;
    }

    printf("FABLETLC_WIDE_STRING_ADD_LEFT PASS\n");
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
