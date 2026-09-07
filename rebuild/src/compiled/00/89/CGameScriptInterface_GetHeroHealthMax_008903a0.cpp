#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CThing;
struct CHero;

extern "C" CThing* __fastcall GetThing(void* p);
extern "C" CHero*  __fastcall ResolveHero(void* p);
extern float g_defaultHealthMax;


float __fastcall CGameScriptInterface_GetHeroHealthMax(CGameScriptInterface* self)
{
    CThing* t = GetThing(self->PlayerManager);
    CHero* h = ResolveHero(t);
    if (h != 0) {
        if ((*(unsigned char*)((char*)h + 0x91) & 1) == 0) {
            return *(float*)((char*)h + 0xb0);
        }
    }
    return g_defaultHealthMax;
}