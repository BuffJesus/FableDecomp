#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char flagC;       // +0x0c
    char pad1[0x0b];
    char f18;         // +0x18
    char pad2[0xbf];
    char fD8;         // +0xd8
};

int g_reset = 0;
int g_second = 0;

void __fastcall CUserProfileManager_OnResetChanged(CUserProfileManager* self) { (void)self; g_reset++; }
void __fastcall CUserProfileManager_OnSecondFlag(CUserProfileManager* self) { (void)self; g_second++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* edx, char a, char b)
{
    (void)edx;
    if (self->flagC)
        self->fD8 = a;
    self->f18 = a;
    CUserProfileManager_OnResetChanged(self);
    if (b)
        CUserProfileManager_OnSecondFlag(self);
}

int main()
{
    CUserProfileManager m;
    m.flagC = 1; m.f18 = 0; m.fD8 = 0;
    CUserProfileManager_SetCameraResetting(&m, 0, (char)5, (char)0);
    if (m.fD8 != 5 || m.f18 != 5 || g_reset != 1 || g_second != 0) { std::printf("FAIL 1\n"); return 1; }

    m.flagC = 0; m.f18 = 0; m.fD8 = 9;
    CUserProfileManager_SetCameraResetting(&m, 0, (char)7, (char)1);
    if (m.fD8 != 9 || m.f18 != 7 || g_reset != 2 || g_second != 1) { std::printf("FAIL 2\n"); return 1; }

    std::printf("CUserProfileManager_0040b250_TEST PASS\n");
    return 0;
}