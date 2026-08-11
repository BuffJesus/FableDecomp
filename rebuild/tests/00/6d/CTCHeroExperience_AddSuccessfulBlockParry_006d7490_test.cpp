#include <cstdio>

struct HelperData {
    char pad[0xc];
    float m0c;
};

int g_blockParryCount = 0;

struct CTCHeroExperience {
    char pad[0x38];
    long m38;

    HelperData* GetBlockParryData();
    void AddSuccessfulBlockParry();
};

static HelperData g_hd;

HelperData* CTCHeroExperience::GetBlockParryData() { return &g_hd; }

void CTCHeroExperience::AddSuccessfulBlockParry()
{
    HelperData* d = this->GetBlockParryData();
    this->m38 = (long)(g_blockParryCount * d->m0c);
}

int main()
{
    CTCHeroExperience h;
    h.m38 = -1;
    g_blockParryCount = 5;
    g_hd.m0c = 3.5f;   // 5 * 3.5 = 17.5 -> ftol truncates to 17
    h.AddSuccessfulBlockParry();
    if (h.m38 == 17) {
        printf("BLOCKPARRY_OK\n");
        return 0;
    }
    printf("BLOCKPARRY_FAIL %ld\n", h.m38);
    return 1;
}