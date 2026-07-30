#include <stdio.h>
#include <string.h>

#include "fable_string.h"

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;

namespace
{
    CWideStringData g_leftStorage;
    CWideStringData g_rightStorage;

    CWideStringData*& Storage(CWideString& value)
    {
        return *reinterpret_cast<CWideStringData**>(&value);
    }
}

void CWideString::UnassignString()
{
    if (storage_ != 0)
    {
        --storage_->owners;
        storage_ = 0;
    }
}

CWideString::CWideString()
    : storage_(0)
{
}

CWideString::~CWideString()
{
}

int main()
{
    memset(&g_leftStorage, 0, sizeof(g_leftStorage));
    memset(&g_rightStorage, 0, sizeof(g_rightStorage));
    g_leftStorage.owners = 1;
    g_rightStorage.owners = 4;

    CWideString left;
    CWideString right;
    Storage(left) = &g_leftStorage;
    Storage(right) = &g_rightStorage;

    left = right;
    if (
        Storage(left) != &g_rightStorage ||
        g_leftStorage.owners != 0 ||
        g_rightStorage.owners != 5)
    {
        return 1;
    }

    left = left;
    if (Storage(left) != &g_rightStorage || g_rightStorage.owners != 5)
        return 2;

    CWideString alias;
    Storage(alias) = &g_rightStorage;
    left = alias;
    if (Storage(left) != &g_rightStorage || g_rightStorage.owners != 5)
        return 3;

    CWideString empty;
    left = empty;
    if (Storage(left) != 0 || g_rightStorage.owners != 4)
        return 4;

    Storage(right) = 0;
    Storage(alias) = 0;
    puts("FABLETLC_WIDE_STRING_COPY_ASSIGNMENT_BEHAVIOR PASS");
    return 0;
}
