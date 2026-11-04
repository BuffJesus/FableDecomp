#include "fable_game.h"
#pragma optimize("s", on)
double CMainGameComponent::GetPredictedTimeSinceLastRenderFrame() const { return GetCurrentGameTime() - LastRenderFrameStartTime; }
