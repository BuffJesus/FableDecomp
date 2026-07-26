#include <stdio.h>
#include <string.h>

#include "fable_string.h"

namespace
{
    CCharStringData g_storage;
    int g_allocationCalls = 0;
    int g_unassignCalls = 0;
    const char* g_observedText = 0;
    long g_observedLength = 0;
}

fable_i32 g_CCharStringInstanceCount_013BD800 = 0;

CCharString::~CCharString()
{
}

CCharStringData* CCharString::AllocStringData(
    const char* text,
    long length)
{
    ++g_allocationCalls;
    g_observedText = text;
    g_observedLength = length;
    return &g_storage;
}

void CCharString::UnassignString()
{
    ++g_unassignCalls;
}

static CCharStringData* ReadStorage(const CCharString& value)
{
    return *reinterpret_cast<CCharStringData* const*>(&value);
}

static void ResetFixture()
{
    g_CCharStringInstanceCount_013BD800 = 0;
    g_allocationCalls = 0;
    g_unassignCalls = 0;
    g_observedText = 0;
    g_observedLength = 0;
}

static int VerifyNullAndEmptyStringsStayUnallocated()
{
    ResetFixture();
    CCharString nullValue(0, -1);
    CCharString emptyValue("", -1);

    if (ReadStorage(nullValue) != 0 || ReadStorage(emptyValue) != 0)
        return 1;
    if (g_allocationCalls != 0)
        return 2;
    if (g_unassignCalls != 0)
        return 9;
    if (g_CCharStringInstanceCount_013BD800 != 2)
        return 3;
    return 0;
}

static int VerifyNonEmptyStringUsesAllocator()
{
    ResetFixture();
    const char commandLineLabel[] = "Parse Command Line";
    CCharString value(commandLineLabel, -1);

    if (ReadStorage(value) != &g_storage)
        return 4;
    if (g_allocationCalls != 1)
        return 5;
    if (g_unassignCalls != 0)
        return 9;
    if (g_observedText != commandLineLabel)
        return 6;
    if (g_observedLength != -1)
        return 7;
    if (g_CCharStringInstanceCount_013BD800 != 1)
        return 8;
    return 0;
}

int main()
{
    int result = VerifyNullAndEmptyStringsStayUnallocated();
    if (result == 0)
        result = VerifyNonEmptyStringUsesAllocator();

    if (result != 0)
    {
        printf("FABLETLC_CHAR_STRING_CONSTRUCTOR_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_CHAR_STRING_CONSTRUCTOR_BEHAVIOR PASS\n");
    return 0;
}
