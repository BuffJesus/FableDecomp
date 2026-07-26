// CTCInventory_CheckSlotTypeNegative @ 0x00c2d7b0 (22 retail bytes)
// Retail: 0f b7 41 0e | 83 e8 00 | 74 0a | 48 | 74 03 | 48 | 75 04 | 83 c8 ff | c3 | 33 c0 | c3
//   movzx eax,WORD PTR [ecx+0xe]  ; read u16 slotType field
//   sub   eax,0     ; je -> return 0   (case 0 / value==0)
//   dec   eax       ; je -> case1 body (or eax,-1)  (value==1)
//   dec   eax       ; jne-> return 0   (default)     ; fallthrough value==2 -> case body
//   or    eax,-1 ; ret            ; return -1  (cases 1 and 2)
//   xor   eax,eax; ret            ; return 0   (case 0 / default)
//
// This is an MSVC small-dense switch lowering on a u16 member. Under VC7.1 the
// decrement-by-1 steps in the switch chain are emitted as `dec eax` (0x48),
// which reproduces the retail bytes exactly. Member function: `this` arrives in
// ecx and there are no stack args (plain `ret`) -> modeled as __fastcall on a
// single pointer arg (VC7.1 has no __fastcall keyword for free functions; the
// unused edx slot is never touched, matching retail).

#include <stddef.h>

struct CTCInventory
{
    unsigned char  pad00[0x0E]; // +0x00 .. +0x0D  (opaque)
    unsigned short slotType;    // +0x0E  the switched-on field
};
// Layout guard: the movzx reads [ecx+0xe].
typedef int _slottype_off_check[(offsetof(CTCInventory, slotType) == 0x0E) ? 1 : -1];

int __fastcall CTCInventory_CheckSlotTypeNegative(CTCInventory* self)
{
    switch (self->slotType)
    {
    case 0:  return 0;
    case 1:  return -1;
    case 2:  return -1;
    default: return 0;
    }
}