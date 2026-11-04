#include "fable_string.h"
#include <stdio.h>
#include <string.h>

fable_i32 g_CCharStringInstanceCount_013BD800;
char g_FableEmptyNarrowString_0129AAF4[1] = {0};

static CCharStringData g_sourceData;
static CCharStringData g_resultData;
static const char* g_allocText;
static long g_allocLength;
static int g_allocCalls;
static int g_unassignCalls;

CCharStringData* CCharString::AllocStringData(const char* text, long length)
{
    ++g_allocCalls;
    g_allocText = text;
    g_allocLength = length;
    g_resultData.owners = 1;
    return &g_resultData;
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

static int run_case(const char* text, unsigned long start, unsigned long end,
                    bool expectAllocation)
{
    unsigned char sourceBytes[sizeof(CCharString)];
    *reinterpret_cast<CCharStringData**>(sourceBytes) = text ? &g_sourceData : 0;
    g_sourceData.Data.PData = const_cast<char*>(text);
    g_sourceData.owners = 1;
    const CCharString& source = *reinterpret_cast<const CCharString*>(sourceBytes);
    g_allocCalls = g_unassignCalls = 0;
    g_allocText = 0;
    g_allocLength = -99;
    {
        CCharString result = source.GetSubString(start, end);
        if (g_CCharStringInstanceCount_013BD800 != 1 || g_unassignCalls != 1)
            return 1;
        if (expectAllocation)
        {
            if (g_allocCalls != 1 || g_allocText != text + start ||
                g_allocLength != static_cast<long>(end - start) ||
                data_of(result) != &g_resultData || g_resultData.owners != 1)
                return 2;
        }
        else if (g_allocCalls != 0 || data_of(result) != 0)
            return 3;
    }
    if (g_CCharStringInstanceCount_013BD800 != 0 || g_unassignCalls != 2)
        return 4;
    if (expectAllocation && g_resultData.owners != 0)
        return 5;
    return 0;
}

int main()
{
    const char text[] = "HelloWorld";
    if (run_case(text, 2, 7, true) != 0) return 1;
    if (run_case(text, 10, 10, false) != 0) return 2;
    if (run_case(0, 0, 0, false) != 0) return 3;
    puts("GETSUBSTRING_OK");
    return 0;
}
