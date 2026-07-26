#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char flag_c;       // +0xc
    char pad_d;        // +0xd
    char field_e;      // +0xe
    char pad2[0xce - 0xf];
    char field_ce;     // +0xce
};

static int g_h1 = 0;
static int g_h2 = 0;

void __fastcall CUPM_Helper1(CUserProfileManager* self) { (void)self; g_h1++; }
void __fastcall CUPM_Helper2(CUserProfileManager* self) { (void)self; g_h2++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* edx, char p1, char p2)
{
    (void)edx;
    if (self->flag_c)
        self->field_ce = p1;
    self->field_e = p1;
    CUPM_Helper1(self);
    if (p2)
        CUPM_Helper2(self);
}

int main()
{
    CUserProfileManager o;
    // case: flag_c set, p1=1, p2=1
    o.flag_c = 1; o.field_e = 0; o.field_ce = 0; g_h1 = g_h2 = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, 1, 1);
    if (o.field_ce != 1 || o.field_e != 1 || g_h1 != 1 || g_h2 != 1) {
        std::printf("FAIL case1\n"); return 1;
    }
    // case: flag_c clear, p1=7, p2=0 -> field_ce untouched, no helper2
    o.flag_c = 0; o.field_e = 0; o.field_ce = 55; g_h1 = g_h2 = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, 7, 0);
    if (o.field_ce != 55 || o.field_e != 7 || g_h1 != 1 || g_h2 != 0) {
        std::printf("FAIL case2\n"); return 1;
    }
    std::printf("CUserProfileManager_0040afd0_TEST PASS\n");
    return 0;
}