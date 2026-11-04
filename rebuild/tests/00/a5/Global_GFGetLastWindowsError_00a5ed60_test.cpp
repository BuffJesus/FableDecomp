#include "fable_string.h"
#include <stdio.h>
#include <string.h>

extern "C" __declspec(dllimport) void __stdcall SetLastError(unsigned long code);

static char g_strings[32][512];
static int g_next_string;
static int g_wide_constructions;
static int g_wide_destructions;
static int g_conversions;

static const char* text_of(const CCharString& value)
{
    unsigned long id = *reinterpret_cast<const unsigned long*>(&value);
    return id ? g_strings[id] : "";
}

static void set_text(CCharString& value, const char* text)
{
    unsigned long id = ++g_next_string;
    strcpy(g_strings[id], text);
    *reinterpret_cast<unsigned long*>(&value) = id;
}

CCharString::CCharString(const char* text, long) : storage_(0) { set_text(*this, text); }
CCharString::CCharString(const CCharString& other) : storage_(other.storage_) {}
CCharString::~CCharString() {}
CWideString::CWideString(const wchar_t* text)
    : storage_(reinterpret_cast<CWideStringData*>(const_cast<wchar_t*>(text)))
{
    ++g_wide_constructions;
}
CWideString::~CWideString() { ++g_wide_destructions; }
CCharString CWideString::ToCharString() const
{
    ++g_conversions;
    const wchar_t* wide = reinterpret_cast<const wchar_t*>(storage_);
    char text[512];
    int index = 0;
    while (wide[index] && index < 510)
    {
        text[index] = wide[index] < 128 ? static_cast<char>(wide[index]) : '?';
        ++index;
    }
    text[index] = 0;
    return CCharString(text);
}

int main()
{
    SetLastError(2);
    CCharString known = GFGetLastWindowsError();
    if (!text_of(known)[0] || g_wide_constructions != 1 ||
        g_wide_destructions != 1 || g_conversions != 1)
        return 1;
    SetLastError(0xffffffffUL);
    CCharString missing = GFGetLastWindowsError();
    if (text_of(missing)[0] || g_wide_constructions != 1 ||
        g_wide_destructions != 1 || g_conversions != 1)
        return 2;
    puts("LAST_WINDOWS_ERROR_OK");
    return 0;
}
