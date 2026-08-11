#include <cstdio>

struct SubObj {
    char pad[0xc];
    float field_c;
};

int g_blockParryInt = 7;   // fild source

struct CTCHeroExperience {
    char pad[0x38];
    int field_38;

    SubObj* GetSub();
    void AddSuccessfulBlockParry();
};

static SubObj g_sub;

SubObj* CTCHeroExperience::GetSub() { return &g_sub; }

void CTCHeroExperience::AddSuccessfulBlockParry()
{
    this->field_38 = (int)( (float)g_blockParryInt * this->GetSub()->field_c );
}

int main()
{
    CTCHeroExperience h;
    h.field_38 = -12345;
    g_sub.field_c = 3.5f;
    g_blockParryInt = 7;      // 7 * 3.5 = 24.5 -> (int) = 24

    h.AddSuccessfulBlockParry();
    if (h.field_38 != 24) { printf("FAIL got %d\n", h.field_38); return 1; }

    g_blockParryInt = 10;
    g_sub.field_c = 2.75f;    // 27.5 -> 27
    h.AddSuccessfulBlockParry();
    if (h.field_38 != 27) { printf("FAIL got %d\n", h.field_38); return 1; }

    printf("BLOCKPARRY_OK\n");
    return 0;
}