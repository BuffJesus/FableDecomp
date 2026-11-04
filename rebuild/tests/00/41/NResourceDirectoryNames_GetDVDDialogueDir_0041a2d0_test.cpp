// Behaviour: GetDVDDialogueDir = L"Data\lang\" + GetLanguageFolder().ToWideString() + L"\",
// built in the hidden return slot, with every temporary (narrow and wide) released.
#include "fable_resource_directories.h"
#include "fable_game_text_bank.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>

static int g_liveWide, g_liveNarrow, g_languageCalls, g_toWideCalls;
static wchar_t* DupW(const wchar_t* s) { size_t n = wcslen(s) + 1; wchar_t* d = (wchar_t*)malloc(n * sizeof(wchar_t)); memcpy(d, s, n * sizeof(wchar_t)); ++g_liveWide; return d; }
static const wchar_t* Text(const CWideString& s) { return *reinterpret_cast<wchar_t* const*>(&s); }
CWideString::CWideString(const wchar_t* text) { *reinterpret_cast<wchar_t**>(this) = DupW(text); }
CWideString::CWideString(const CWideString& other) { *reinterpret_cast<wchar_t**>(this) = DupW(Text(other)); }
CWideString::~CWideString() { free(*reinterpret_cast<wchar_t**>(this)); --g_liveWide; }
CWideString FABLE_FASTCALL operator+(const CWideString& left, const wchar_t* right)
{ wchar_t b[260]; wcscpy(b, Text(left)); wcscat(b, right); return CWideString(b); }
CWideString FABLE_FASTCALL operator+(const wchar_t* left, const CWideString& right)
{ wchar_t b[260]; wcscpy(b, left); wcscat(b, Text(right)); return CWideString(b); }

CCharString::CCharString(const char* text, long) { *reinterpret_cast<const char**>(this) = text; ++g_liveNarrow; }
CCharString::~CCharString() { --g_liveNarrow; }
CWideString CCharString::ToWideString() const
{
    ++g_toWideCalls;
    const char* t = *reinterpret_cast<const char* const*>(this);
    wchar_t b[64]; size_t i = 0; for (; t[i] && i < 63; ++i) b[i] = (wchar_t)t[i]; b[i] = 0;
    return CWideString(b);
}
CCharString FABLE_FASTCALL NLocalisation::GetLanguageFolder() { ++g_languageCalls; return CCharString("English"); }

int main()
{
    {
        CWideString dir = NResourceDirectoryNames::GetDVDDialogueDir();
        if (wcscmp(Text(dir), L"Data\\lang\\English\\") != 0) { wprintf(L"FAIL got %s\n", Text(dir)); return 1; }
        if (g_languageCalls != 1 || g_toWideCalls != 1) { printf("FAIL calls\n"); return 2; }
        if (g_liveWide != 1 || g_liveNarrow != 0) { printf("FAIL leaks wide=%d narrow=%d\n", g_liveWide, g_liveNarrow); return 3; }
    }
    if (g_liveWide != 0) { printf("FAIL result not released\n"); return 4; }
    printf("FABLETLC_RESDIR_GETDVDDIALOGUEDIR PASS\n");
    return 0;
}