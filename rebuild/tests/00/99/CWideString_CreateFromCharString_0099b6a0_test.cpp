#include <stdio.h>

#include "fable_string.h"

namespace
{
    CWideStringData g_storage;
    const CCharString* g_observedSource = 0;
    int g_conversionConstructorCalls = 0;
}

CWideString::CWideString(const CCharString& text)
{
    ++g_conversionConstructorCalls;
    g_observedSource = &text;
    *reinterpret_cast<CWideStringData**>(this) = &g_storage;
}

CWideString::~CWideString()
{
}

static CWideStringData* ReadStorage(const CWideString& value)
{
    return *reinterpret_cast<CWideStringData* const*>(&value);
}

int main()
{
    CCharString source;
    CWideString result = CWideString::CreateFromCharString(source);

    if (g_conversionConstructorCalls != 1)
        return 1;
    if (g_observedSource != &source)
        return 2;
    if (ReadStorage(result) != &g_storage)
        return 3;

    puts("FABLETLC_WIDE_STRING_FROM_CHAR_FACTORY_BEHAVIOR PASS");
    return 0;
}

CCharString::CCharString()
{
}

CCharString::~CCharString()
{
}
