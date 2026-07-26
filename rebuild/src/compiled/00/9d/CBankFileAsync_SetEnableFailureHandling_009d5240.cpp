#include "fable_async.h"

void FABLE_FASTCALL CBankFileAsync::SetEnableFailureHandling(bool enabled)
{
    g_CBankFileAsyncFailureHandlingMode_013CA868 = enabled ? 0 : 2;
}
