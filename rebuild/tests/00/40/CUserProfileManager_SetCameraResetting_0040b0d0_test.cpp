#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char flag_c;       // +0x0c
    char pad_d[0x12-0x0d];
    char field_12;     // +0x12
    char pad2[0xd2-0x13];
    char field_d2;     // +0xd2
};

static int g_sub1 = 0;
static int g_sub2 = 0;

void __fastcall CUserProfileManager_ResetSub(CUserProfileManager* self) { g_sub1++; }
void __fastcall CUserProfileManager_ResetSub2(CUserProfileManager* self) { g_sub2++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, int edx_, char p1, char p2)
{
    if (self->flag_c)
        self->field_d2 = p1;
    self->field_12 = p1;
    CUserProfileManager_ResetSub(self);
    if (p2)
        CUserProfileManager_ResetSub2(self);
}

int main() {
    CUserProfileManager o;
    // case A: flag_c set, p2 set
    o.flag_c = 1; o.field_12 = 0; o.field_d2 = 0;
    g_sub1 = g_sub2 = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, (char)7, (char)1);
    if (o.field_d2 != 7 || o.field_12 != 7 || g_sub1 != 1 || g_sub2 != 1) {
        std::printf("FAIL A\n"); return 1;
    }
    // case B: flag_c clear, p2 clear
    o.flag_c = 0; o.field_12 = 0; o.field_d2 = 55;
    g_sub1 = g_sub2 = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, (char)9, (char)0);
    if (o.field_d2 != 55 || o.field_12 != 9 || g_sub1 != 1 || g_sub2 != 0) {
        std::printf("FAIL B\n"); return 1;
    }
    std::printf("CUserProfileManager_0040b0d0_TEST PASS\n");
    return 0;
}