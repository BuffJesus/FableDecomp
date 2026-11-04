#pragma optimize("s", on)
#include "fable_game.h"

// Compile the optional Xbox definitions first, then the PC definitions.
void FABLE_FASTCALL CNewFrontendGameComponent::CompileDefs()
{
    if (g_FableCompileFrontendDefinitions_013B8648)
        CompileDefsPlatformSpecific(true);
    CompileDefsPlatformSpecific(false);
}
