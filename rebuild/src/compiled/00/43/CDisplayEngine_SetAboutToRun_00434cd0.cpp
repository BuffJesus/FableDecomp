#include "fable_display_engine.h"
#include "fable_profile.h"
void CDisplayEngine::SetAboutToRun() {
    if (!ScreenFadeOutLocked && GInitialFadeAllowedToBeActive) {
        CFadeInFadeOutBase& fade=ScreenFadeOutInfo;
        fade.FadeOutTime=3.0f;
        fade.CloseTimer=3.0f;
        fade.FadeInTime=0;
        fade.Active=true;
        fade.Closing=false;
        fade.Opening=true;
        fade.OpenTimer=0;
        fade.ToColour=CRGBColour(0,0,0);
        fade.Update(0.1f);
        fade.Opening=false;
        fade.Closing=true;
        ScreenFadeDuration=-1.0f;
        TimeSinceFadeStarted=0;
        InitialFadeActive=true;
    }
    NProfileTimer::EndProfile(1);
}
