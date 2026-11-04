#include "fable_game.h"
float CMainGameComponent::GetAverageFrameDuration() const { float t = static_cast<float>(FrameRateSmoother.GetSmoothedTime()); return t; }
