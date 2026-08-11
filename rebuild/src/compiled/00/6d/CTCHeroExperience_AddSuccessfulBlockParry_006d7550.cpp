// CTCHeroExperience::AddSuccessfulBlockParry  @ 0x006d7550

struct SubObj {
    char pad[0xc];
    float field_c;   // dword ptr [eax+0xc]
};

extern int g_blockParryInt;  // fild dword ptr [0x1375550]

struct CTCHeroExperience {
    char pad[0x38];
    int field_38;    // [esi+0x38]

    SubObj* GetSub();                 // call 0x6d72d0 (thiscall, returns SubObj*)
    void AddSuccessfulBlockParry();
};

void CTCHeroExperience::AddSuccessfulBlockParry()
{
    this->field_38 = (int)( (float)g_blockParryInt * this->GetSub()->field_c );
}