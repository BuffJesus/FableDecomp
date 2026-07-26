#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char flagC;
    char pad1[0x50-0x0d];
    char resetFlag50;
    char pad2[0x10c-0x51];
    char field10c;
};

static int g_apply = 0;
static int g_notify = 0;
void __fastcall CUserProfileManager_Apply(CUserProfileManager* self) { (void)self; g_apply++; }
void __fastcall CUserProfileManager_Notify(CUserProfileManager* self) { (void)self; g_notify++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* /*edx*/, char resetting, char notify)
{
    if (self->flagC)
        self->field10c = resetting;
    self->resetFlag50 = resetting;
    CUserProfileManager_Apply(self);
    if (notify)
        CUserProfileManager_Notify(self);
}

int main() {
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); ++i) ((char*)&o)[i] = 0;
    o.flagC = 1;
    g_apply = g_notify = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, (char)7, (char)1);
    if (o.field10c != 7 || o.resetFlag50 != 7 || g_apply != 1 || g_notify != 1) { std::printf("FAIL case1\n"); return 1; }

    CUserProfileManager o2;
    for (int i = 0; i < (int)sizeof(o2); ++i) ((char*)&o2)[i] = 0;
    o2.flagC = 0;
    o2.field10c = 99;
    g_apply = g_notify = 0;
    CUserProfileManager_SetCameraResetting(&o2, 0, (char)3, (char)0);
    if (o2.field10c != 99 || o2.resetFlag50 != 3 || g_apply != 1 || g_notify != 0) { std::printf("FAIL case2\n"); return 1; }

    std::printf("CUserProfileManager_0040b750_TEST PASS\n");
    return 0;
}