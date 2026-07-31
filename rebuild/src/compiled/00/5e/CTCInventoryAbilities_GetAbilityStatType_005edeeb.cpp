#pragma optimize("s",on)
struct Entry { void* a; struct Ability* b; };
struct Ability { char pad[0x40]; int statType; };
struct CTCInventoryAbilities { char pad[0x160]; Entry* table; };

int __fastcall GetAbilityStatType(CTCInventoryAbilities* self, void*, int index)
{
    Ability* ab = self->table[index].b;
    if (ab)
        return ab->statType;
    return 2;
}