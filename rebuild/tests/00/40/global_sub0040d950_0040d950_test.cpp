#include "rebuild_abi.h"
#include <stdio.h>

extern "C" void* __fastcall sub_0040d950(void* thisptr);

int main()
{
    int dummy = 0;
    void* p = (void*)&dummy;
    void* r = sub_0040d950(p);
    if (r == p)
        printf("OK_0040d950\n");
    else
        printf("FAIL_0040d950\n");
    return 0;
}