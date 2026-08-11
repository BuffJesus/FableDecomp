// CTCHeroExperience::GetCategoryTrainableStatIsIn @ 006d6850

enum EHeroTrainableStatType { };
enum EHeroExperienceStatCategory { };

struct StatEntry {
    char pad0[0x3c];        // 0x00..0x3b
    int  category;          // 0x3c
    char pad1[80 - 0x40];   // pad to 80 bytes total
};

struct StatTable {
    char pad0[0xfc];        // 0x00..0xfb
    StatEntry* entries;     // 0xfc
};

struct CTCHeroExperience {
    char pad0[0x5c];        // 0x00..0x5b
    StatTable* table;       // 0x5c
    EHeroExperienceStatCategory GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat);
};

EHeroExperienceStatCategory CTCHeroExperience::GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat)
{
    return (EHeroExperienceStatCategory)this->table->entries[(int)stat].category;
}