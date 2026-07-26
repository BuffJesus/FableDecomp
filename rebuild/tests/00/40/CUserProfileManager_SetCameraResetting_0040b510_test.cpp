#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char gate;
    char pad1[0x79 - 0xd];
    char flag79;
    char pad2[0x12c - 0x7a];
    char flag12c;
};

static int g_a = 0, g_b = 0;
void __fastcall CUserProfileManager_HelperA(CUserProfileManager* self) { (void)self; g_a++; }
void __fastcall CUserProfileManager_HelperB(CUserProfileManager* self) { (void)self; g_b++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* /*edx*/, char p1, char p2)
{
    if (self->gate)
        self->flag12c = p1;
    self->flag79 = p1;
    CUserProfileManager_HelperA(self);
    if (p2)
        CUserProfileManager_HelperB(self);
}

int main()
{
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); ++i) ((char*)&o)[i] = 0;

    /* gate off: flag12c untouched, flag79 set, HelperA called, p2 off */
    o.gate = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, (char)7, (char)0);
    if (o.flag12c != 0) { std::printf("FAIL gate-off wrote 12c\n"); return 1; }
    if (o.flag79 != 7) { std::printf("FAIL flag79\n"); return 1; }
    if (g_a != 1) { std::printf("FAIL helperA\n"); return 1; }
    if (g_b != 0) { std::printf("FAIL helperB called\n"); return 1; }

    /* gate on + p2 on */
    o.gate = 1;
    CUserProfileManager_SetCameraResetting(&o, 0, (char)5, (char)1);
    if (o.flag12c != 5) { std::printf("FAIL gate-on 12c\n"); return 1; }
    if (o.flag79 != 5) { std::printf("FAIL flag79 b\n"); return 1; }
    if (g_a != 2) { std::printf("FAIL helperA b\n"); return 1; }
    if (g_b != 1) { std::printf("FAIL helperB not called\n"); return 1; }

    std::printf("CUserProfileManager_0040b510_TEST PASS\n");
    return 0;
}