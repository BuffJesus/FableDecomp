#include "engine/CInventoryItemDef.h"  // retyped onto the PDB layout; byte parity re-verified
extern void __fastcall CInventoryItemDef_Copy_impl(CInventoryItemDef* self, void* src);
void __fastcall CInventoryItemDef_Copy(CInventoryItemDef* self, void* src) {
    CInventoryItemDef_Copy_impl(self, src);
}