#include "fable_game.h"
#pragma optimize("s", on)
void CMainGameComponent::UpdateAverageFrameDuration(double duration) { FrameRateSmoother.AddFrameTime(duration); }
