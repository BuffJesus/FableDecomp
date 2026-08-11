#include <cstdio>

enum EHeroTrainableStatType { };
enum EHeroExperienceStatCategory { };

struct StatEntry {
    char pad0[0x3c];
    int  category;
    char pad1[80 - 0x40];
};
struct StatTable {
    char pad0[0xfc];
    StatEntry* entries;
};
struct CTCHeroExperience {
    char pad0[0x5c];
    StatTable* table;
    EHeroExperienceStatCategory GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat);
};

EHeroExperienceStatCategory CTCHeroExperience::GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat)
{
    return (EHeroExperienceStatCategory)this->table->entries[(int)stat].category;
}

int main() {
    StatEntry ents[4];
    for (int i = 0; i < 4; ++i) ents[i].category = 100 + i;
    StatTable tbl;
    tbl.entries = ents;
    CTCHeroExperience h;
    h.table = &tbl;

    int ok = 1;
    for (int i = 0; i < 4; ++i) {
        int r = (int)h.GetCategoryTrainableStatIsIn((EHeroTrainableStatType)i);
        if (r != 100 + i) ok = 0;
    }
    if (ok) printf("CATEGORY_LOOKUP_OK\n");
    else printf("FAIL\n");
    return ok ? 0 : 1;
}