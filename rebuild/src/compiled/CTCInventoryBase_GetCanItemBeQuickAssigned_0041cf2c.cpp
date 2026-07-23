// NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned @ 0x0041cf2c
// Retail: 8a 81 c8 00 00 00 c3  ->  mov al,BYTE PTR [ecx+0xc8]; ret
//
// This is a this-only byte accessor. Modeled as __fastcall so the object
// pointer arrives in ECX (byte-identical to __fastcall for a this-only
// member). Reading the bool field at +0xC8 into AL and returning it
// yields exactly `mov al,[ecx+0xc8]; ret` under cl /O2.

struct CTCInventoryBase
{
    char _pad[0xC8];      // 0x00 .. 0xC7
    bool CanItemBeQuickAssigned; // 0xC8
};

// __fastcall puts the first (only) arg in ECX == this.
bool __fastcall CTCInventoryBase_GetCanItemBeQuickAssigned(CTCInventoryBase *self)
{
    return self->CanItemBeQuickAssigned;
}