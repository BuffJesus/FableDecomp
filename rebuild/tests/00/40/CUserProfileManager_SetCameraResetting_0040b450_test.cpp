#include <cstdio>

struct CUserProfileManager {
    char pad00[0x0c];
    unsigned char m_flag_0c;
    char pad0d[0x32 - 0x0d];
    unsigned char m_byte_32;
    char pad33[0xf1 - 0x33];
    unsigned char m_byte_f1;
};

int g_a = 0;
int g_b = 0;

void __fastcall UPM_MethodA(CUserProfileManager* self) { (void)self; g_a++; }
void __fastcall UPM_MethodB(CUserProfileManager* self) { (void)self; g_b++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, int edx, char p1, char p2)
{
    (void)edx;
    if (self->m_flag_0c)
        self->m_byte_f1 = (unsigned char)p1;
    self->m_byte_32 = (unsigned char)p1;
    UPM_MethodA(self);
    if (p2)
        UPM_MethodB(self);
}

int main() {
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); ++i) ((char*)&o)[i] = 0;

    // case: flag_0c set, p1=1, p2=1
    o.m_flag_0c = 1;
    g_a = g_b = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, 1, 1);
    if (o.m_byte_f1 != 1) { std::printf("FAIL f1\n"); return 1; }
    if (o.m_byte_32 != 1) { std::printf("FAIL b32\n"); return 1; }
    if (g_a != 1) { std::printf("FAIL a\n"); return 1; }
    if (g_b != 1) { std::printf("FAIL b\n"); return 1; }

    // case: flag_0c clear, p1=1 -> f1 untouched, p2=0 -> no B
    for (int i = 0; i < (int)sizeof(o); ++i) ((char*)&o)[i] = 0;
    g_a = g_b = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, 1, 0);
    if (o.m_byte_f1 != 0) { std::printf("FAIL f1b\n"); return 1; }
    if (o.m_byte_32 != 1) { std::printf("FAIL b32b\n"); return 1; }
    if (g_a != 1) { std::printf("FAIL a2\n"); return 1; }
    if (g_b != 0) { std::printf("FAIL b2\n"); return 1; }

    std::printf("CUserProfileManager_0040b450_TEST PASS\n");
    return 0;
}