// Member getter at this+0x0. __fastcall this=ecx. pack(1) keeps the Height at
// the retail byte offset (natural alignment would shift an odd one).
#include "engine/CHeightMapCell.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CHeightMapCell_Methods : CHeightMapCell {
    float Get();
};
#pragma pack(pop)
float CHeightMapCell_Methods::Get() { return this->Height; }