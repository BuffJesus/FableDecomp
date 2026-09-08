#include "engine/CTCBuyableHouse.h"

struct CTCBuyableHouse_Methods : CTCBuyableHouse {
    void SetDressLevel(int lvl);
};

extern int GetNewThing();

void __fastcall CTCBuyableHouse_IncrementDressLevel(CTCBuyableHouse* self)
{
    ((CTCBuyableHouse_Methods*)self)->SetDressLevel(self->CurrentDressLevel + 1);
    self->FrameToNextCheckIfWeNeedToCreateAnything = GetNewThing();
}
