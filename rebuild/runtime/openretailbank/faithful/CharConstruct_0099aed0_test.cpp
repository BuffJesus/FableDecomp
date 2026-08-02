
#include <cstdio>
#include "fable_string.h"

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;
fable_i32 g_CCharStringInstanceCount_013BD800 = 0;
wchar_t g_FableEmptyWideString_0129A8E0[1] = {0};

CWideString::CWideString()
    : storage_(0)
{
    ++g_CWideStringInstanceCount_013BCA20;
}

CWideString::~CWideString()
{
}

int main() {
    fable_i32 before = g_CWideStringInstanceCount_013BCA20;
    CWideString obj;
    fable_i32 after = g_CWideStringInstanceCount_013BCA20;
    if (after == before + 1) {
        printf("CWIDESTRING_CTOR_OK\n");
    } else {
        printf("CWIDESTRING_CTOR_FAIL\n");
    }
    return 0;
}
