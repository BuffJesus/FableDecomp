struct CTCInventoryBase { char pad[0x9a]; unsigned char canQuickAssign; };

unsigned char __fastcall CTCInventoryBase_GetCanItemBeQuickAssigned(const CTCInventoryBase* self)
{
    return self->canQuickAssign;
}