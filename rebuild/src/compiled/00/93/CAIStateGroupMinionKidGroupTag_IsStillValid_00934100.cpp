// CAIStateGroup_MinionKidGroupTag_Methods::IsStillValid  @ 0x00934100
// disasm:
//   mov ecx,[ecx+0x18]; test ecx,ecx; je false; call 0x8138e0;
//   test eax,eax; je false; mov eax,1; ret; false: xor eax,eax; ret

#include "engine/CAIStateGroup_MinionKidGroupTag.h"  // retyped onto the PDB layout; byte parity re-verified
struct CTCCreatureGroupTag {
    int CheckValid();    // __fastcall @ 0x8138e0, ecx=this
};

struct CAIStateGroup_MinionKidGroupTag_Methods : CAIStateGroup_MinionKidGroupTag {
    // +0x18
    int IsStillValid();
};

int CAIStateGroup_MinionKidGroupTag_Methods::IsStillValid()
{
    CTCCreatureGroupTag* p = this->PTCGroupTag;
    if (p != 0 && p->CheckValid())
        return 1;
    return 0;
}