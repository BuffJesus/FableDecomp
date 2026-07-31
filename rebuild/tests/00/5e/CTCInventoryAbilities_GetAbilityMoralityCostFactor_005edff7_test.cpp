#include <stdio.h>
struct AbilityEntry { void* a; void* ability; };
struct AbilityObj { char pad[0x90]; float costFactor; };
float g_defaultMoralityCostFactor = 2.5f;
struct CTCInventoryAbilities {
    char pad[0x160];
    AbilityEntry* table;
    float GetAbilityMoralityCostFactor(int index);
};
float CTCInventoryAbilities::GetAbilityMoralityCostFactor(int index)
{
    AbilityObj* ab = (AbilityObj*)table[index].ability;
    if (ab)
        return ab->costFactor;
    return g_defaultMoralityCostFactor;
}
int main()
{
    AbilityObj ob;
    ob.costFactor = 7.25f;
    AbilityEntry entries[3];
    entries[0].a = 0; entries[0].ability = 0;
    entries[1].a = 0; entries[1].ability = &ob;
    entries[2].a = 0; entries[2].ability = 0;
    CTCInventoryAbilities inv;
    inv.table = entries;

    float present = inv.GetAbilityMoralityCostFactor(1);
    float absent  = inv.GetAbilityMoralityCostFactor(0);

    if (present == 7.25f && absent == 2.5f)
        printf("OK_0x005edff7 present=%f absent=%f\n", present, absent);
    else
        printf("BAD present=%f absent=%f\n", present, absent);
    return 0;
}