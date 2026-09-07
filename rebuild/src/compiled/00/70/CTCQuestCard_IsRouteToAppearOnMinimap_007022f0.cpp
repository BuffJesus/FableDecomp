#include "engine/CTCQuestCard.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefPointeeBase {
    char pad[0x59];
    char b59;
    char b5a;
    char b5b;
};


int __fastcall CTCQuestCard_IsRouteToAppearOnMinimap(CTCQuestCard* self)
{
    CDefPointeeBase* p = self->PDef_Object;
    if (!p->b5b)
    {
        if (p->b59)
            return 0;
    }
    return 1;
}