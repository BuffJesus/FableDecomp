#include "fable_async.h"
#include <stdio.h>

int main()
{
    if (g_CBankFileAsyncFailureHandlingMode_013CA868 != 0) return 1;
    for (unsigned long i = 0; i < 16; ++i) {
        g_CBankFileAsyncFailureHandlingMode_013CA868 = i * 0x01010101ul;
        if (g_CBankFileAsyncFailureHandlingMode_013CA868 != i * 0x01010101ul)
            return 2;
    }
    puts("BANK_ASYNC_FAILURE_MODE_PASS cases=16");
    return 0;
}
