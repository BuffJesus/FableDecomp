#include "fable_game.h"
extern "C" __declspec(dllimport) unsigned long FABLE_STDCALL timeGetTime();
void CRoughFrameCounter::NewFrame() {
    unsigned long now = timeGetTime();
    unsigned long elapsed = now - FrameStart;
    if (elapsed) FPS = static_cast<float>(1000.0 / static_cast<float>(elapsed));
    else FPS = 0.0f;
    FrameStart = now;
}