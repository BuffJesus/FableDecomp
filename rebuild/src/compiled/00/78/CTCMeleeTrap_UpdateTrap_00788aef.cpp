#pragma optimize("s",on)
// CTCMeleeTrap::UpdateTrap @ 0x00788aef  (__fastcall, self in ecx)
// mov eax,[ecx+0x54]; fld [eax+0x44]; push ecx;push ecx;
// fstp [esp+4]; fld [0x123078c]; fstp [esp]; call helper; ret

struct SubObj {
    char pad[0x44];
    float f44;
};

extern float g_someConst;

void __stdcall MeleeTrapHelper(float a, float b);

struct CTCMeleeTrap {
    char pad[0x54];
    SubObj* m54;
    void UpdateTrap();
};

void CTCMeleeTrap::UpdateTrap()
{
    MeleeTrapHelper(g_someConst, this->m54->f44);
}