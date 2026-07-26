#include "rebuild_abi.h"

// ~CGlobalDef (0x00458a4f): destroy embedded sub-objects at fixed offsets,
// then tail-call the base-class destructor. Modeled instruction-for-instruction:
// each member dtor is a this-only member (free __fastcall), and the final base
// dtor is a tail jump.

struct CGlobalDef;

// Member sub-object destructors (distinct callees).
extern "C" void __fastcall CGlobalDef_dtor_10c(void* thisptr); // [esi+0x10c]
extern "C" void __fastcall CGlobalDef_dtor_100(void* thisptr); // [esi+0x100]
extern "C" void __fastcall CGlobalDef_dtor_0f4(void* thisptr); // [esi+0xf4]
extern "C" void __fastcall CGlobalDef_dtor_shared(void* thisptr); // 0x546091, reused for ac/98/94/90

// Base-class destructor (tail-called).
extern "C" void __fastcall CGlobalDef_base_dtor(void* thisptr);

extern "C" void __fastcall CGlobalDef_dtor(CGlobalDef* thisptr)
{
    unsigned char* esi = reinterpret_cast<unsigned char*>(thisptr);

    CGlobalDef_dtor_10c(esi + 0x10c);
    CGlobalDef_dtor_100(esi + 0x100);
    CGlobalDef_dtor_0f4(esi + 0xf4);
    CGlobalDef_dtor_shared(esi + 0xac);
    CGlobalDef_dtor_shared(esi + 0x98);
    CGlobalDef_dtor_shared(esi + 0x94);
    CGlobalDef_dtor_shared(esi + 0x90);

    // mov ecx, esi ; jmp base_dtor  (tail call)
    CGlobalDef_base_dtor(esi);
}