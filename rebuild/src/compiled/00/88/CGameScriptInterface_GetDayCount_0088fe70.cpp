#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner2 { unsigned char _p[0x14]; long day; };
struct CWorld { unsigned char _p[0x1c]; Inner2* p1c; };

long __fastcall CGameScriptInterface_GetDayCount(const CGameScriptInterface* self) {
    return self->World->p1c->day;
}