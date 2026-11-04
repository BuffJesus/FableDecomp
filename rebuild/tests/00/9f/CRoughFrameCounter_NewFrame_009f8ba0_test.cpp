#include "fable_game.h"
#include <stdio.h>
#pragma comment(linker, "/alternatename:__imp__timeGetTime@0=_g_TestTimeGetTime")
static unsigned long now;
static unsigned calls;
static unsigned long FABLE_STDCALL Clock() { ++calls; return now; }
extern "C" unsigned long (FABLE_STDCALL *g_TestTimeGetTime)() = &Clock;
int main() {
    const unsigned long start[] = {100, 100, 0xFFFFFFFEUL, 0};
    const unsigned long end[] = {100, 125, 2, 0x80000000UL};
    const float fps[] = {0.0f, 40.0f, 250.0f, static_cast<float>(1000.0 / 2147483648.0)};
    for (int i = 0; i < 4; ++i) {
        unsigned long storage[4] = {0xA5A5A5A5UL,0,0,0xA5A5A5A5UL};
        CRoughFrameCounter* counter = reinterpret_cast<CRoughFrameCounter*>(storage+1);
        counter->FrameStart=start[i]; counter->FPS=-1.0f; now=end[i]; calls=0;
        counter->NewFrame();
        if (counter->FrameStart!=now || counter->FPS!=fps[i] || calls!=1) return 1;
        if (storage[0]!=0xA5A5A5A5UL || storage[3]!=0xA5A5A5A5UL) return 2;
    }
    puts("FABLETLC_ROUGH_FRAME_NEWFRAME PASS"); return 0;
}