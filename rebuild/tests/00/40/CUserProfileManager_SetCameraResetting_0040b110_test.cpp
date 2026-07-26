#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char flag0c;
    char pad1[0x13 - 0xd];
    char b13;
    char pad2[0xd3 - 0x14];
    char bd3;
};

static int g_update = 0;
static int g_refresh = 0;

void __fastcall UPM_Update(CUserProfileManager* self) { (void)self; g_update++; }
void __fastcall UPM_Refresh(CUserProfileManager* self) { (void)self; g_refresh++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, int edx_pad, char p1, char p2)
{
    (void)edx_pad;
    if (self->flag0c)
        self->bd3 = p1;
    self->b13 = p1;
    UPM_Update(self);
    if (p2)
        UPM_Refresh(self);
}

int main()
{
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); i++) ((char*)&o)[i] = 0;

    o.flag0c = 1;
    CUserProfileManager_SetCameraResetting(&o, 0, 7, 1);
    if (o.bd3 != 7) { std::printf("FAIL bd3\n"); return 1; }
    if (o.b13 != 7) { std::printf("FAIL b13\n"); return 1; }
    if (g_update != 1) { std::printf("FAIL update\n"); return 1; }
    if (g_refresh != 1) { std::printf("FAIL refresh\n"); return 1; }

    CUserProfileManager o2;
    for (int i = 0; i < (int)sizeof(o2); i++) ((char*)&o2)[i] = 0;
    o2.bd3 = 99;
    CUserProfileManager_SetCameraResetting(&o2, 0, 3, 0);
    if (o2.bd3 != 99) { std::printf("FAIL bd3 unchanged\n"); return 1; }
    if (o2.b13 != 3) { std::printf("FAIL b13_2\n"); return 1; }
    if (g_update != 2) { std::printf("FAIL update2\n"); return 1; }
    if (g_refresh != 1) { std::printf("FAIL refresh2\n"); return 1; }

    std::printf("CUserProfileManager_0040b110_TEST PASS\n");
    return 0;
}