#include <cstdio>

struct CPersistContext { int marker; };

// Model: transfer writes back into *pValue (deserialize direction).
void __fastcall PC_Transfer(CPersistContext* pc, int /*value*/, int* pValue)
{
    *pValue = pc->marker;
}

struct EHeroAbility_slot { int value; };
struct Self { void* pad0; EHeroAbility_slot* dst; };

void __fastcall TransferIn(Self* self, int /*edx*/, CPersistContext* pc, int value)
{
    EHeroAbility_slot* dst = self->dst;
    PC_Transfer(pc, value, &value);
    dst->value = value;
}

int main()
{
    EHeroAbility_slot slot; slot.value = -1;
    Self self; self.pad0 = 0; self.dst = &slot;
    CPersistContext pc; pc.marker = 42;

    TransferIn(&self, 0, &pc, 7);  // passed value 7 overwritten to 42 by transfer
    if (slot.value == 42)
        printf("PARITY_OK_0057f93e\n");
    else
        printf("FAIL %d\n", slot.value);
    return 0;
}