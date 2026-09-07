#include "engine/CAIStateGroup_JackOfBladesStage2.h"  // retyped onto the PDB layout; byte parity re-verified
struct CObjB;

// 0x6b3950 : thiscall member, two stack args, cleans own stack
struct CObjC {
    void Sub_6B3950(int a, unsigned int b);
};

struct CObjB {
    char pad[0x18];
    CObjC* field18; // +0x18
};


// 0x972110 : member on self, result discarded
extern void __fastcall Sub_972110(CAIStateGroup_JackOfBladesStage2* self);
// 0x8fcfc0 : member on self, returns object pointer
extern CObjB* __fastcall Sub_8FCFC0(CAIStateGroup_JackOfBladesStage2* self);

void __fastcall Cleanup(CAIStateGroup_JackOfBladesStage2* self)
{
    Sub_972110(self);
    CObjB* b = Sub_8FCFC0(self);
    b->field18->Sub_6B3950(3, 0x40000000u);
}