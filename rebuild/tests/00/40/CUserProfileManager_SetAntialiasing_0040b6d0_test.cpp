#include <cstdio>

struct CUserProfileManager {
    char pad0[0x0c];
    char flag0c;
    char pad1[0x4c - 0x0d];
    int val4c;
    char pad2[0x108 - 0x50];
    int val108;
};

static int g_apply = 0;
static int g_notify = 0;

void __fastcall CUserProfileManager_Apply(CUserProfileManager* self) { (void)self; g_apply++; }
void __fastcall CUserProfileManager_Notify(CUserProfileManager* self) { (void)self; g_notify++; }

void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, void* edx, int value, char doNotify)
{
    if (self->flag0c)
        self->val108 = value;
    self->val4c = value;
    CUserProfileManager_Apply(self);
    if (doNotify)
        CUserProfileManager_Notify(self);
}

int main()
{
    CUserProfileManager o;
    // case: flag set, notify set
    o.flag0c = 1; o.val4c = 0; o.val108 = 0; g_apply = 0; g_notify = 0;
    CUserProfileManager_SetAntialiasing(&o, 0, 7, 1);
    if (o.val108 != 7 || o.val4c != 7 || g_apply != 1 || g_notify != 1) { std::printf("FAIL a\n"); return 1; }

    // case: flag clear, notify clear
    o.flag0c = 0; o.val4c = 0; o.val108 = 99; g_apply = 0; g_notify = 0;
    CUserProfileManager_SetAntialiasing(&o, 0, 5, 0);
    if (o.val108 != 99 || o.val4c != 5 || g_apply != 1 || g_notify != 0) { std::printf("FAIL b\n"); return 1; }

    std::printf("CUserProfileManager_0040b6d0_TEST PASS\n");
    return 0;
}