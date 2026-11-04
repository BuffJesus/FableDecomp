#include "fable_string.h"
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

fable_i32 g_CCharStringInstanceCount_013BD800;
static CCharStringData g_formattedData;
static int g_sprintfCalls;
static int g_unassignCalls;
static int g_seenValue;
static const char* g_seenFormat;

void CCharString::Sprintf(const char* format, ...)
{
    ++g_sprintfCalls;
    g_seenFormat = format;
    va_list args;
    va_start(args, format);
    g_seenValue = va_arg(args, int);
    va_end(args);
    *reinterpret_cast<CCharStringData**>(this) = &g_formattedData;
    g_formattedData.owners = 1;
}

void CCharString::UnassignString()
{
    ++g_unassignCalls;
    CCharStringData*& data = *reinterpret_cast<CCharStringData**>(this);
    if (data != 0)
    {
        --data->owners;
        data = 0;
    }
}

static CCharStringData* data_of(const CCharString& value)
{
    return *reinterpret_cast<CCharStringData* const*>(&value);
}

static int run_case(int value)
{
    g_sprintfCalls = g_unassignCalls = 0;
    g_seenFormat = 0;
    {
        CCharString output = GFIntToCharString(value);
        if (g_sprintfCalls != 1 || !g_seenFormat ||
            strcmp(g_seenFormat, "%d") != 0 || g_seenValue != value ||
            data_of(output) != &g_formattedData || g_formattedData.owners != 1 ||
            g_unassignCalls != 1 || g_CCharStringInstanceCount_013BD800 != 1)
            return 1;
    }
    return g_unassignCalls == 2 && g_formattedData.owners == 0 &&
           g_CCharStringInstanceCount_013BD800 == 0 ? 0 : 2;
}

int main()
{
    if (run_case(0)) return 1;
    if (run_case(17)) return 2;
    if (run_case(-42)) return 3;
    if (run_case(INT_MIN)) return 4;
    puts("GFINT_TO_CHARSTRING_OK");
    return 0;
}
