#include <stdio.h>

#include "fable_definition_table.h"

namespace
{
    const CWideString* g_assignedSource = 0;
}

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;

CWideString::CWideString()
{
}

CWideString::~CWideString()
{
}

CWideString& CWideString::operator=(const CWideString& other)
{
    g_assignedSource = &other;
    return *this;
}

int main()
{
    CDefStringTableRecovered table;
    CWideString pathname;
    g_assignedSource = 0;

    table.SetTablePath(pathname);
    if (g_assignedSource != &pathname)
    {
        printf("FABLETLC_DEF_TABLE_PATH_BEHAVIOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_DEF_TABLE_PATH_BEHAVIOR PASS\n");
    return 0;
}
