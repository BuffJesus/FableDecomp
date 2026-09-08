#include "engine/CTCBuyableHouse.h"
#include <stdio.h>

struct CTCBuyableHouse_Methods : CTCBuyableHouse {
    void SetDressLevel(int lvl);
};

static int g_lastLvl = -999;
void CTCBuyableHouse_Methods::SetDressLevel(int lvl) { g_lastLvl = lvl; }
int GetNewThing() { return 0x1234; }

void __fastcall CTCBuyableHouse_IncrementDressLevel(CTCBuyableHouse* self);

int main()
{
    CTCBuyableHouse_Methods h;
    h.CurrentDressLevel = 41;
    h.FrameToNextCheckIfWeNeedToCreateAnything = 0;
    CTCBuyableHouse_IncrementDressLevel(&h);
    if (g_lastLvl == 42 && h.FrameToNextCheckIfWeNeedToCreateAnything == 0x1234)
        printf("OK_006c04b0\n");
    else
        printf("FAIL lvl=%d f34=%x\n", g_lastLvl, h.FrameToNextCheckIfWeNeedToCreateAnything);
    return 0;
}
