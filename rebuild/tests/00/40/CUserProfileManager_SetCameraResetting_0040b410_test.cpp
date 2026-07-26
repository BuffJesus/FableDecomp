#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char flag0c;
    char pad_d[0x30-0xd];
    char flag30;
    char pad_31[0xf0-0x31];
    char flagf0;
};

int g_n1 = 0;
int g_n2 = 0;

void __fastcall CUPM_Notify1(CUserProfileManager* self) { (void)self; g_n1++; }
void __fastcall CUPM_Notify2(CUserProfileManager* self) { (void)self; g_n2++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* edx, char p1, char p2)
{
    (void)edx;
    if (self->flag0c)
        self->flagf0 = p1;
    self->flag30 = p1;
    CUPM_Notify1(self);
    if (p2)
        CUPM_Notify2(self);
}

int main()
{
    CUserProfileManager m;
    m.flag0c = 0; m.flag30 = 0; m.flagf0 = 0;
    g_n1 = 0; g_n2 = 0;
    CUserProfileManager_SetCameraResetting(&m, 0, 1, 0);
    if (m.flagf0 != 0) { std::printf("FAIL flagf0 set when flag0c=0\n"); return 1; }
    if (m.flag30 != 1) { std::printf("FAIL flag30\n"); return 1; }
    if (g_n1 != 1) { std::printf("FAIL notify1\n"); return 1; }
    if (g_n2 != 0) { std::printf("FAIL notify2 called\n"); return 1; }

    m.flag0c = 1; m.flag30 = 0; m.flagf0 = 0;
    g_n1 = 0; g_n2 = 0;
    CUserProfileManager_SetCameraResetting(&m, 0, 1, 1);
    if (m.flagf0 != 1) { std::printf("FAIL flagf0\n"); return 1; }
    if (m.flag30 != 1) { std::printf("FAIL flag30 b\n"); return 1; }
    if (g_n1 != 1) { std::printf("FAIL notify1 b\n"); return 1; }
    if (g_n2 != 1) { std::printf("FAIL notify2 b\n"); return 1; }

    std::printf("CUserProfileManager_0040b410_TEST PASS\n");
    return 0;
}