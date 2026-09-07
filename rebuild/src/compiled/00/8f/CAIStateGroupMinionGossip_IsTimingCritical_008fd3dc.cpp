// Member getter at this+0x18. __fastcall this=ecx. pack(1) keeps the Critical at
// the retail byte offset (natural alignment would shift an odd one).
#include "engine/CAIStateGroup_MinionGossip.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CAIStateGroup_MinionGossip_Methods : CAIStateGroup_MinionGossip {
    char Get();
};
#pragma pack(pop)
char CAIStateGroup_MinionGossip_Methods::Get() { return this->Critical; }