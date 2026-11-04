#include "fable_fade_in_out.h"
void CFadeInFadeOutBase::Update(float elapsed) {
    if (Opening) {
        OpenTimer+=elapsed;
        if (OpenTimer>=FadeInTime) Opening=false;
    } else if (Closing) {
        CloseTimer-=elapsed;
        if (CloseTimer<=0) {
            if (CloseTimer<0) CloseTimer=0;
            Active=false;
            Closing=false;
            Opening=false;
        }
    }
}
