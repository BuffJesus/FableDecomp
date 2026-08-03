#pragma optimize("s",on)
struct AbilityEntry { void* a; void* ability; };
struct AbilityObj { char pad[0x90]; float costFactor; };
extern float g_defaultMoralityCostFactor;
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