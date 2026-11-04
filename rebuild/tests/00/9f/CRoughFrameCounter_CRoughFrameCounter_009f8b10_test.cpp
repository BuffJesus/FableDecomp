#include "fable_game.h"
#include <new>
#include <stdio.h>
int main() {
    unsigned long storage[4] = {0xA5A5A5A5UL,0xA5A5A5A5UL,0xA5A5A5A5UL,0xA5A5A5A5UL};
    CRoughFrameCounter* counter = new (storage+1) CRoughFrameCounter;
    if (counter->FrameStart || counter->FPS != 0.0f || storage[1] || storage[2]) return 1;
    if (storage[0]!=0xA5A5A5A5UL || storage[3]!=0xA5A5A5A5UL) return 2;
    puts("PASS_009f8b10"); return 0;
}
