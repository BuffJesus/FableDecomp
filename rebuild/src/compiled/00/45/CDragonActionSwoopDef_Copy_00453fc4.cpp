#include "engine/CDragonActionSwoopDef.h"  // retyped onto the PDB layout; byte parity re-verified
extern void __fastcall CDragonActionSwoopDef_Copy_impl(CDragonActionSwoopDef* self, const CDragonActionSwoopDef* other);
void __fastcall CDragonActionSwoopDef_Copy(CDragonActionSwoopDef* self, const CDragonActionSwoopDef* other)
{
    CDragonActionSwoopDef_Copy_impl(self, other);
}