#include <stdio.h>
#include <string.h>

#include "fable_string.h"

fable_i32 g_CCharStringInstanceCount_013BD800 = 0;

namespace
{
    CCharStringData g_leftStorage;
    CCharStringData g_rightStorage;

    CCharStringData*& Storage(CCharString& value)
    {
        return *reinterpret_cast<CCharStringData**>(&value);
    }
}

void CCharString::UnassignString()
{
    if (storage_ != 0)
    {
        --storage_->owners;
        storage_ = 0;
    }
}

CCharString::CCharString()
    : storage_(0)
{
}

CCharString::~CCharString()
{
}

int main()
{
    memset(&g_leftStorage, 0, sizeof(g_leftStorage));
    memset(&g_rightStorage, 0, sizeof(g_rightStorage));
    g_leftStorage.owners = 1;
    g_rightStorage.owners = 4;

    CCharString left;
    CCharString right;
    Storage(left) = &g_leftStorage;
    Storage(right) = &g_rightStorage;

    left = right;
    if (Storage(left) != &g_rightStorage ||
        g_leftStorage.owners != 0 ||
        g_rightStorage.owners != 5)
    {
        printf("FABLETLC_CHAR_STRING_COPY_ASSIGNMENT_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    left = left;
    if (Storage(left) != &g_rightStorage ||
        g_rightStorage.owners != 5)
    {
        printf("FABLETLC_CHAR_STRING_COPY_ASSIGNMENT_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    Storage(left) = 0;
    Storage(right) = 0;
    printf("FABLETLC_CHAR_STRING_COPY_ASSIGNMENT_BEHAVIOR PASS\n");
    return 0;
}
