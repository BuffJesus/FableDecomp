#include "fable_string.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

fable_i32 g_CCharStringInstanceCount_013BD800;
static char g_captured[128];
static int g_constructions;
static int g_releases;

template<> CBasicString<char>::CBasicString(const char* text)
{
    ++g_constructions;
    strcpy(g_captured, text);
    PData = g_captured;
    StringLength = static_cast<unsigned long>(strlen(text));
    DataLength = StringLength;
    UseFastExtend = true;
}

void CCharString::UnassignString()
{
    ++g_releases;
    CCharStringData*& data = *reinterpret_cast<CCharStringData**>(this);
    if (data != 0)
    {
        if (--data->owners <= 0)
            delete data;
        data = 0;
    }
}

static CCharStringData* data_of(const CCharString& value)
{
    return *reinterpret_cast<CCharStringData* const*>(&value);
}

static int run_case(int value)
{
    char expected[64];
    sprintf(expected, "%d", value);
    int constructedBefore = g_constructions;
    int releasedBefore = g_releases;
    {
        CCharString result = GFIntToCharString(value);
        if (strcmp(g_captured, expected) != 0 ||
            g_constructions != constructedBefore + 1 ||
            g_releases != releasedBefore + 1 ||
            !data_of(result) || data_of(result)->owners != 1 ||
            g_CCharStringInstanceCount_013BD800 != 1)
            return 1;
    }
    return g_releases == releasedBefore + 2 &&
           g_CCharStringInstanceCount_013BD800 == 0 ? 0 : 2;
}

int main()
{
    if (run_case(0)) return 1;
    if (run_case(7)) return 2;
    if (run_case(-42)) return 3;
    if (run_case(INT_MIN)) return 4;
    if (run_case(INT_MAX)) return 5;
    puts("GFINT_COMPOSED_OK");
    return 0;
}
