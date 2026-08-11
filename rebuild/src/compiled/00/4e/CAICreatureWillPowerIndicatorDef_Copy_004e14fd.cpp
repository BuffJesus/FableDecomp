// CAICreatureWillPowerIndicatorDef::Copy @ 004e14fd
struct CDefClassBase;

// Base with an implicit-thiscall member (this->ecx, other->stack, rel32 call).
struct CAICreatureWillPowerIndicatorDefBase {
    void CloneFields(const CDefClassBase* other); // extern, implicit __fastcall
};

struct CAICreatureWillPowerIndicatorDef : public CAICreatureWillPowerIndicatorDefBase {
    char pad[0x28];
    unsigned long field28;
};

void __fastcall CAICreatureWillPowerIndicatorDef_Copy(
    CAICreatureWillPowerIndicatorDef* self, int /*edx*/, const CDefClassBase* other)
{
    self->CloneFields(other);
    self->field28 = reinterpret_cast<const CAICreatureWillPowerIndicatorDef*>(other)->field28;
}