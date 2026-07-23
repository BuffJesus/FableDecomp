struct CDragonActionBaseDef {
    unsigned char _pad_0x0[0x28];
    void Copy(const CDragonActionBaseDef* other);
};
struct CDragonActionHoverDef : public CDragonActionBaseDef {
    int field28; // +0x28
};
void __fastcall CDragonActionHoverDef_Copy(CDragonActionHoverDef* self, void* edx, const CDragonActionHoverDef* other)
{
    self->CDragonActionBaseDef::Copy(other);
    self->field28 = other->field28;
}