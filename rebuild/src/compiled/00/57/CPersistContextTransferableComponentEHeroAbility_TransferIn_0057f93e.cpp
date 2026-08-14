// CPersistContext_TransferableComponent<EHeroAbility>::TransferIn  @ 0x0057f93e
#pragma optimize("y", off)   // retail keeps the EBP frame here
#pragma optimize("s", on)    // favor size -> reload temp lands in eax (retail regalloc)
struct CPersistContext;

// pc->Transfer(value, &value): __fastcall, ecx=pc, edx=value, stack=&value.
void __fastcall PC_Transfer(CPersistContext* pc, int value, int* pValue);

struct EHeroAbility_slot { int value; };

struct Self {
    void*              pad0;  // +0
    EHeroAbility_slot* dst;   // +4
};

void __fastcall TransferIn(Self* self, int /*edx*/, CPersistContext* pc, int value)
{
    EHeroAbility_slot* dst = self->dst;
    PC_Transfer(pc, value, &value);
    dst->value = value;
}