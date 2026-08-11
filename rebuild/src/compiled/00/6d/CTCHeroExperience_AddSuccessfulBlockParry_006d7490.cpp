// CTCHeroExperience::AddSuccessfulBlockParry  @ 0x006d7490

struct HelperData {
    char pad[0xc];
    float m0c;   // [eax+0xc]
};

extern int g_blockParryCount;  // fild dword ptr [0x1375550]

struct CTCHeroExperience {
    char pad[0x38];
    long m38;    // [esi+0x38]

    HelperData* GetBlockParryData();   // call 0x6d72d0 (__fastcall, returns ptr)

    void AddSuccessfulBlockParry();
};

void CTCHeroExperience::AddSuccessfulBlockParry()
{
    HelperData* d = this->GetBlockParryData();
    this->m38 = (long)(g_blockParryCount * d->m0c);
}