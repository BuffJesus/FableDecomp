// Member setter at this+0x25: one stack arg, `ret 4`. __fastcall this=ecx.
#include "engine/CParentDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CParentDefClassBase_Methods : CParentDefClassBase {
    void Set(int value);
};
#pragma pack(pop)
void CParentDefClassBase_Methods::Set(int value) { this->InstantiationName_TablePos = value; }