#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char flag;
    char pad1[0x2c - 0xd];
    int m2c;
    char pad2[0xec - 0x30];
    int mec;
};

static int g_h1 = 0;
static int g_h2 = 0;

void __fastcall CUPM_Helper1(CUserProfileManager* self) { (void)self; ++g_h1; }
void __fastcall CUPM_Helper2(CUserProfileManager* self) { (void)self; ++g_h2; }

void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, void* edx, volatile int value, char apply)
{
    (void)edx;
    if (self->flag)
        self->mec = value;
    self->m2c = value;
    CUPM_Helper1(self);
    if (apply)
        CUPM_Helper2(self);
}

int main() {
    CUserProfileManager o;
    o.flag = 1; o.m2c = 0; o.mec = 0;
    CUserProfileManager_SetAntialiasing(&o, 0, 42, 1);
    if (o.mec != 42) { std::printf("FAIL mec\n"); return 1; }
    if (o.m2c != 42) { std::printf("FAIL m2c\n"); return 1; }
    if (g_h1 != 1) { std::printf("FAIL h1\n"); return 1; }
    if (g_h2 != 1) { std::printf("FAIL h2\n"); return 1; }

    g_h1 = g_h2 = 0;
    o.flag = 0; o.m2c = 0; o.mec = 7;
    CUserProfileManager_SetAntialiasing(&o, 0, 99, 0);
    if (o.mec != 7) { std::printf("FAIL mec2\n"); return 1; }
    if (o.m2c != 99) { std::printf("FAIL m2c2\n"); return 1; }
    if (g_h1 != 1) { std::printf("FAIL h1b\n"); return 1; }
    if (g_h2 != 0) { std::printf("FAIL h2b\n"); return 1; }

    std::printf("CUserProfileManager_0040b3d0_TEST PASS\n");
    return 0;
}