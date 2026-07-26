#include <stdio.h>

#include "fable_async.h"

fable_u32 g_CBankFileAsyncFailureHandlingMode_013CA868 = 0;

int main()
{
    CBankFileAsync::SetEnableFailureHandling(false);
    if (g_CBankFileAsyncFailureHandlingMode_013CA868 != 2)
    {
        printf("FABLETLC_ASYNC_FAILURE_HANDLING_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    CBankFileAsync::SetEnableFailureHandling(true);
    if (g_CBankFileAsyncFailureHandlingMode_013CA868 != 0)
    {
        printf("FABLETLC_ASYNC_FAILURE_HANDLING_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_ASYNC_FAILURE_HANDLING_BEHAVIOR PASS\n");
    return 0;
}
