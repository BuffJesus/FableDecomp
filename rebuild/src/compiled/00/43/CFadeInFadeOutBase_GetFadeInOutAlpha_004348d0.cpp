#include "fable_fade_in_out.h"
float CFadeInFadeOutBase::GetFadeInOutAlpha()
{
    if (Opening) {
        if (FadeInTime < 0.0001f) return 1.0f;
        return OpenTimer / FadeInTime;
    }
    if (Closing) {
        if (FadeOutTime < 0.0001f) return 1.0f;
        return CloseTimer / FadeOutTime;
    }
    return Active ? 1.0f : 0.0f;
}
