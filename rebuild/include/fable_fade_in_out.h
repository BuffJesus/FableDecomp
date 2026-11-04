#pragma once
#include "rebuild_abi.h"
#include "fable_rgb_colour.h"

// Ego_r.pdb layout; retail display-engine construction inlines this lifetime.
class CFadeInFadeOutBase {
public:
    float GetFadeInOutAlpha(); // retail004348D0
    void Update(float elapsed); // retail00434870
    CFadeInFadeOutBase() {
        Active=false;
        CloseTimer=0;OpenTimer=0;
        Opening=false;Closing=false;
        ToColour=CRGBColour(0,0,0);
    }
    bool Active;
    float FadeInTime,FadeOutTime;
    bool Closing,Opening;
    float OpenTimer,CloseTimer;
    CRGBColour ToColour;
};
class CLetterBoxModeInfo : public CFadeInFadeOutBase {
public:
    float Ratio;
};
FABLE_STATIC_ASSERT(sizeof(CFadeInFadeOutBase)==0x1C);
FABLE_STATIC_ASSERT(sizeof(CLetterBoxModeInfo)==0x20);
