#include <cstdio>

struct CUserProfileManager {
    char pad0[0x0c];
    char flagC;        // +0x0c
    char pad1[0x38 - 0x0d];
    int fld38;         // +0x38
    char pad2[0xf4 - 0x3c];
    int fldF4;         // +0xf4
};

static int g_apply = 0;
static int g_refresh = 0;

extern "C" void __fastcall UPM_Apply(CUserProfileManager* self) { (void)self; ++g_apply; }
extern "C" void __fastcall UPM_Refresh(CUserProfileManager* self) { (void)self; ++g_refresh; }

void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, int /*edx*/, int value, char doRefresh)
{
    if (self->flagC)
        self->fldF4 = value;
    self->fld38 = value;
    UPM_Apply(self);
    if (doRefresh)
        UPM_Refresh(self);
}

int main()
{
    CUserProfileManager m;
    for (int i = 0; i < (int)sizeof(m); ++i) ((char*)&m)[i] = 0;

    // Case 1: flagC set, doRefresh set
    m.flagC = 1; m.fld38 = 0; m.fldF4 = 0;
    g_apply = 0; g_refresh = 0;
    CUserProfileManager_SetAntialiasing(&m, 0, 7, 1);
    if (m.fldF4 != 7 || m.fld38 != 7 || g_apply != 1 || g_refresh != 1) {
        std::printf("FAIL case1\n"); return 1;
    }

    // Case 2: flagC clear -> fldF4 untouched, doRefresh clear -> no refresh
    m.flagC = 0; m.fld38 = 0; m.fldF4 = 99;
    g_apply = 0; g_refresh = 0;
    CUserProfileManager_SetAntialiasing(&m, 0, 42, 0);
    if (m.fldF4 != 99 || m.fld38 != 42 || g_apply != 1 || g_refresh != 0) {
        std::printf("FAIL case2\n"); return 1;
    }

    std::printf("CUserProfileManager_0040b4d0_TEST PASS\n");
    return 0;
}