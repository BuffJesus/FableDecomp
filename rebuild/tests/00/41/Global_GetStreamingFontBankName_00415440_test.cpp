#include <stdio.h>
#include <string.h>

#include "fable_language.h"

namespace
{
    CCharStringData g_storage;
    fable_i32 g_language = 0;
    int g_constructorCalls = 0;
    int g_destructorCalls = 0;
    const char* g_observedText = 0;
    long g_observedLength = 0;
}

fable_i32 FableGetCurrentLanguage_00415070()
{
    return g_language;
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

static int CheckCase(fable_i32 language, const char* expected)
{
    g_language = language;
    g_observedText = 0;
    g_observedLength = 0;

    {
        CCharString result =
            FableGetStreamingFontBankName_00415440();
        if (g_observedText == 0 ||
            strcmp(g_observedText, expected) != 0)
            return 1;
        if (g_observedLength != -1)
            return 2;
        if (*reinterpret_cast<CCharStringData* const*>(&result) !=
            &g_storage)
            return 3;
    }

    return 0;
}

int main()
{
    static const struct
    {
        fable_i32 language;
        const char* text;
    } cases[] = {
        {4, "STREAMING_FONT_CHINESE_PC"},
        {7, "STREAMING_FONT_GERMAN_PC"},
        {10, "STREAMING_FONT_SPANISH_PC"},
        {12, "STREAMING_FONT_ITALIAN_PC"},
        {16, "STREAMING_FONT_FRENCH_PC"},
        {17, "STREAMING_FONT_JAPANESE_PC"},
        {18, "STREAMING_FONT_KOREAN_PC"},
        {0, "STREAMING_FONT_ENGLISH_PC"},
        {19, "STREAMING_FONT_ENGLISH_PC"},
    };

    for (unsigned int index = 0;
         index != sizeof(cases) / sizeof(cases[0]);
         ++index)
    {
        const int result =
            CheckCase(cases[index].language, cases[index].text);
        if (result != 0)
            return 10 + result;
    }

    if (g_constructorCalls != 9 || g_destructorCalls != 9)
        return 20;

    puts("FABLETLC_STREAMING_FONT_BANK_NAME_BEHAVIOR PASS");
    return 0;
}
