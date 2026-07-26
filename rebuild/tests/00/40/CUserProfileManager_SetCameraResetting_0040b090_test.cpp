#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char m_flag0c;
    char pad_d[0x11-0xc-1];
    char m_val11;
    char pad_11[0xd1-0x11-1];
    char m_valD1;
};

static int g_sub1 = 0;
static int g_sub2 = 0;

void __fastcall CUserProfileManager_Sub1(CUserProfileManager* self) { (void)self; g_sub1++; }
void __fastcall CUserProfileManager_Sub2(CUserProfileManager* self) { (void)self; g_sub2++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* edx, char a, char b)
{
    if (self->m_flag0c)
        self->m_valD1 = a;
    self->m_val11 = a;
    CUserProfileManager_Sub1(self);
    if (b)
        CUserProfileManager_Sub2(self);
}

int main()
{
    CUserProfileManager m;
    // Case 1: flag set, b set
    m.m_flag0c = 1; m.m_valD1 = 0; m.m_val11 = 0; g_sub1 = 0; g_sub2 = 0;
    CUserProfileManager_SetCameraResetting(&m, 0, (char)7, (char)1);
    if (m.m_valD1 != 7 || m.m_val11 != 7 || g_sub1 != 1 || g_sub2 != 1) { std::printf("FAIL case1\n"); return 1; }

    // Case 2: flag clear -> no valD1 store; b clear -> no sub2
    m.m_flag0c = 0; m.m_valD1 = 99; m.m_val11 = 0; g_sub1 = 0; g_sub2 = 0;
    CUserProfileManager_SetCameraResetting(&m, 0, (char)5, (char)0);
    if (m.m_valD1 != 99 || m.m_val11 != 5 || g_sub1 != 1 || g_sub2 != 0) { std::printf("FAIL case2\n"); return 1; }

    std::printf("CUserProfileManager_0040b090_TEST PASS\n");
    return 0;
}