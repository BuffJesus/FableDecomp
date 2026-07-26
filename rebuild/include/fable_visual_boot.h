#pragma once

#include "fable_boot.h"

// Authored reconstruction checkpoint, not a recovered retail renderer.
// It proves that the matched WinMain and reconstructed GFMain phases can hand
// control to a real Win32 window while the retail display path is recovered.
long FABLE_FASTCALL FableRunVisualBootCheckpoint(
    FableInstanceHandle instance,
    char* commandLine,
    int showCommand);
