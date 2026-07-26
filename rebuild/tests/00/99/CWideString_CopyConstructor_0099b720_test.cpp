#include <stdio.h>

#include "fable_string.h"

namespace
{
    int g_unassignCalls = 0;
}

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;

void CWideString::UnassignString()
{
    ++g_unassignCalls;
}

CWideString::~CWideString()
{
    --g_CWideStringInstanceCount_013BCA20;
}

static void InstallStorage(
    CWideString& value,
    CWideStringData* storage)
{
    *reinterpret_cast<CWideStringData**>(&value) = storage;
}

static CWideStringData* ReadStorage(const CWideString& value)
{
    return *reinterpret_cast<CWideStringData* const*>(&value);
}

int main()
{
    CWideStringData sharedStorage;
    sharedStorage.text = 0;
    sharedStorage.unknown04 = 0;
    sharedStorage.unknown08 = 0;
    sharedStorage.owners = 3;

    {
        CWideString source;
        InstallStorage(source, &sharedStorage);

        {
            CWideString copy(source);
            if (ReadStorage(copy) != &sharedStorage)
                return 1;
            if (sharedStorage.owners != 4)
                return 2;
            if (g_CWideStringInstanceCount_013BCA20 != 2)
                return 3;
            if (g_unassignCalls != 0)
                return 4;
        }

        InstallStorage(source, 0);
    }

    if (g_CWideStringInstanceCount_013BCA20 != 0)
        return 5;

    puts("FABLETLC_WIDE_STRING_COPY_CONSTRUCTOR_BEHAVIOR PASS");
    return 0;
}

CWideString::CWideString()
    : storage_(0)
{
    ++g_CWideStringInstanceCount_013BCA20;
}
