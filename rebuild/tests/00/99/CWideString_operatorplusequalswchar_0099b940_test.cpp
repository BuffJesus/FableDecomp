// Behaviour of CWideString::operator+=(const wchar_t*) 0x0099B940: a string
// with data first takes its own copy of the data (MakeOwnCopyOfData), then
// appends the literal to it (even an empty one: no change); an empty string
// takes new data (AllocStringData(text, -1)) only for a non-null, non-empty
// literal. Returns itself.
#include "fable_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

void _STL::_String_base<wchar_t, _STL::allocator<wchar_t> >::_M_throw_length_error() const
{
    puts("length error");
    exit(1);
}

static char g_log[128];
static void Log(const char* t) { strcat(g_log, t); }
static CWideStringData* g_allocated;
static const wchar_t* g_allocText;
static long g_allocLength;
static CWideStringData*& Storage(CWideString& s) { return *reinterpret_cast<CWideStringData**>(&s); }

void CWideString::MakeOwnCopyOfData() { Log("own;"); }
CWideStringData* CWideString::AllocStringData(const wchar_t* text, long length)
{
    Log("alloc;");
    g_allocText = text;
    g_allocLength = length;
    return g_allocated;
}

int main()
{
    static unsigned char store[sizeof(CWideString)];
    CWideString& s = *reinterpret_cast<CWideString*>(store);
    CWideStringData* data = new CWideStringData(L"abc");
    Storage(s) = data;
    // With data: own copy, then append.
    if (&(s += L"de") != &s || strcmp(g_log, "own;") != 0 || Storage(s) != data ||
        wcscmp(data->Data.c_str(), L"abcde") != 0) { printf("append: %s\n", g_log); return 1; }
    // With data, empty literal: own copy only; the text is unchanged.
    g_log[0] = 0;
    s += L"";
    if (strcmp(g_log, "own;") != 0 || wcscmp(data->Data.c_str(), L"abcde") != 0) { printf("empty literal: %s\n", g_log); return 1; }
    // No data: a new data block for a non-empty literal.
    g_log[0] = 0;
    Storage(s) = 0;
    static unsigned char fresh[sizeof(CWideStringData)];
    g_allocated = reinterpret_cast<CWideStringData*>(fresh);
    const wchar_t* text = L"xyz";
    if (&(s += text) != &s || strcmp(g_log, "alloc;") != 0 || Storage(s) != g_allocated || g_allocText != text ||
        g_allocLength != -1) { printf("alloc: %s\n", g_log); return 1; }
    // No data, empty or null literal: nothing.
    g_log[0] = 0;
    Storage(s) = 0;
    s += L"";
    s += (const wchar_t*)0;
    if (g_log[0] || Storage(s) != 0) { printf("nothing: %s\n", g_log); return 1; }
    puts("FABLETLC_CWIDESTRING_APPEND_LITERAL_0099B940_PASS");
    return 0;
}
