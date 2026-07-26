#pragma once

#include "rebuild_abi.h"

class CBankFileAsync
{
public:
    static void FABLE_FASTCALL SetEnableFailureHandling(bool enabled);
};

// The retail flag is encoded as a two-state failure policy rather than a
// literal bool: 0 enables normal handling, while 2 suppresses it.
extern fable_u32 g_CBankFileAsyncFailureHandlingMode_013CA868;
