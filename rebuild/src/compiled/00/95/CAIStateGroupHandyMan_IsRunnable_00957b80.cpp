#include "engine/CAIStateGroup_HandyMan.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_HandyMan_Methods : CAIStateGroup_HandyMan {
    bool IsRunnable();
};

// helper at 0x91a6f0: __fastcall(this) -> bool
bool __fastcall helper_91a6f0(CAIStateGroup_HandyMan* self);
// tail at 0x91b010: __fastcall(this) -> bool
bool __fastcall tail_91b010(CAIStateGroup_HandyMan* self);

bool CAIStateGroup_HandyMan_Methods::IsRunnable()
{
    if (helper_91a6f0(this))
        return tail_91b010(this);
    return false;
}