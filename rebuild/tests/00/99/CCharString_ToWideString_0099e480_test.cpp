#include <stdio.h>

#include "fable_string.h"

namespace
{
    CWideStringData g_storage;
    const CCharString* g_observedSource = 0;
    int g_factoryCalls = 0;
    int g_copyCalls = 0;
    int g_destructorCalls = 0;
}

CWideString FABLE_FASTCALL CWideString::CreateFromCharString(
    const CCharString& text)
{
    ++g_factoryCalls;
    g_observedSource = &text;
    return CWideString(text);
}

CWideString::CWideString(const CCharString&)
{
    *reinterpret_cast<CWideStringData**>(this) = &g_storage;
}

CWideString::CWideString(const CWideString& other)
{
    ++g_copyCalls;
    *reinterpret_cast<CWideStringData**>(this) =
        *reinterpret_cast<CWideStringData* const*>(&other);
}

CWideString::~CWideString()
{
    ++g_destructorCalls;
}

static CWideStringData* ReadStorage(const CWideString& value)
{
    return *reinterpret_cast<CWideStringData* const*>(&value);
}

int main()
{
    CCharString source;

    {
        CWideString result = source.ToWideString();
        if (g_factoryCalls != 1)
            return 1;
        if (g_observedSource != &source)
            return 2;
        if (g_copyCalls != 1)
            return 3;
        if (g_destructorCalls != 1)
            return 4;
        if (ReadStorage(result) != &g_storage)
            return 5;
    }

    if (g_destructorCalls != 2)
        return 6;

    puts("FABLETLC_CHAR_TO_WIDE_STRING_BEHAVIOR PASS");
    return 0;
}

CCharString::CCharString()
{
}

CCharString::~CCharString()
{
}
