#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    unsigned char flagC;
    char pad1[0x9];
    unsigned char byte16;
    char pad2[0xbf];
    unsigned char byteD6;
};

int g_a = 0;
int g_b = 0;
void __fastcall UPM_helperA(CUserProfileManager*) { g_a++; }
void __fastcall UPM_helperB(CUserProfileManager*) { g_b++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* /*edx*/, char p1, char p2)
{
    if (self->flagC)
        self->byteD6 = (unsigned char)p1;
    self->byte16 = (unsigned char)p1;
    UPM_helperA(self);
    if (p2)
        UPM_helperB(self);
}

int main()
{
    CUserProfileManager o;
    o.flagC = 1; o.byte16 = 0; o.byteD6 = 0;
    g_a = 0; g_b = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, (char)7, (char)0);
    if (o.byteD6 != 7 || o.byte16 != 7 || g_a != 1 || g_b != 0) { std::printf("FAIL case1\n"); return 1; }

    o.flagC = 0; o.byte16 = 0; o.byteD6 = 99;
    g_a = 0; g_b = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, (char)5, (char)1);
    if (o.byteD6 != 99 || o.byte16 != 5 || g_a != 1 || g_b != 1) { std::printf("FAIL case2\n"); return 1; }

    std::printf("CUserProfileManager_0040b1d0_TEST PASS\n");
    return 0;
}