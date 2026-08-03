struct AbilityEntry {
    int level;
    int pad;
};

struct CTCInventoryAbilities {
    char pad[0x160];
    AbilityEntry* abilities; // at +0x160
    int GetAbilityLevel(int index);
};

int CTCInventoryAbilities::GetAbilityLevel(int index) {
    return this->abilities[index].level;
}