struct CTCInventoryBase {
    char pad[0x34];
    bool canQuickAssign;
};

bool __fastcall CTCInventoryBase_GetCanItemBeQuickAssigned(const CTCInventoryBase* self)
{
    return self->canQuickAssign;
}