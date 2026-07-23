#include <stdio.h>
#include "rebuild_abi.h"

void FABLE_STDCALL $E2(void* value);

int main()
{
    int value = 7;
    $E2(&value);
    if (value != 7) return 1;
    printf("GLOBAL_E2_TEST PASS\n");
    return 0;
}
