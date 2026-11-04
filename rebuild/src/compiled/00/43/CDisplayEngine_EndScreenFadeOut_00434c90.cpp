#include "fable_display_engine.h"
void CDisplayEngine::EndScreenFadeOut()
{
    InitialFadeActive = false;
    GInitialFadeAllowedToBeActive = false;
    if (ScreenFadeOutInfo.Active && !ScreenFadeOutLocked)
    {
        ScreenFadeDuration = -1.0f;
        TimeSinceFadeStarted = 0.0f;
        ScreenFadeOutInfo.Opening = false;
        ScreenFadeOutInfo.Closing = true;
    }
}
