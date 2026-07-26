#include <stdio.h>
#include <string.h>

#include "fable_language.h"

namespace
{
    CCharStringData g_storage;
    int g_constructorCalls = 0;
    int g_destructorCalls = 0;
    const char* g_observedText = 0;
    long g_observedLength = 0;
}

CCharString::CCharString(const char* text, long length)
{
    ++g_constructorCalls;
    g_observedText = text;
    g_observedLength = length;
    *reinterpret_cast<CCharStringData**>(this) = &g_storage;
}

CCharString::~CCharString()
{
    ++g_destructorCalls;
}

static CCharStringData* ReadStorage(const CCharString& value)
{
    return *reinterpret_cast<CCharStringData* const*>(&value);
}

int main()
{
    {
        CCharString result = FableGetDefaultLanguageName_00415530();

        if (g_constructorCalls != 1)
            return 1;
        if (g_observedText == 0 || strcmp(g_observedText, "English") != 0)
            return 2;
        if (g_observedLength != -1)
            return 3;
        if (ReadStorage(result) != &g_storage)
            return 4;
        if (g_destructorCalls != 0)
            return 5;
    }

    if (g_destructorCalls != 1)
        return 6;

    puts("FABLETLC_DEFAULT_LANGUAGE_NAME_BEHAVIOR PASS");
    return 0;
}
