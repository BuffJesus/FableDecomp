// Store -1 into a field of the PDrawMouseCursor_Data-object pointer at this+0x248. __fastcall this=ecx.
#include "engine/CPlayerGui.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CDrawMouseCursor { char pad[0x2c]; int field; };
struct CPlayerGui_Methods : CPlayerGui {
    void Hide();
};
#pragma pack(pop)
void CPlayerGui_Methods::Hide() { this->PDrawMouseCursor_Data->field = -1; }