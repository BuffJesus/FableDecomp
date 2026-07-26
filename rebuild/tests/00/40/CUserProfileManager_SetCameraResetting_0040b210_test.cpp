#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char m_flagC;      // +0xc
    char pad1[0xa];
    char m_flag17;     // +0x17
    char pad2[0xbf];
    char m_flagD7;     // +0xd7
};

static int g_h1 = 0;
static int g_h2 = 0;

void __fastcall CUserProfileManager_Helper1(CUserProfileManager* self) { g_h1++; }
void __fastcall CUserProfileManager_Helper2(CUserProfileManager* self) { g_h2++; }

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, int edx_dummy, char p1, char p2)
{
    if (self->m_flagC)
        self->m_flagD7 = p1;
    self->m_flag17 = p1;
    CUserProfileManager_Helper1(self);
    if (p2)
        CUserProfileManager_Helper2(self);
}

int main()
{
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); i++) ((char*)&o)[i] = 0;

    // Case: flagC set, p2 set
    o.m_flagC = 1;
    g_h1 = g_h2 = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, 5, 7);
    if (o.m_flagD7 != 5) { std::printf("FAIL d7\n"); return 1; }
    if (o.m_flag17 != 5) { std::printf("FAIL 17\n"); return 1; }
    if (g_h1 != 1) { std::printf("FAIL h1\n"); return 1; }
    if (g_h2 != 1) { std::printf("FAIL h2\n"); return 1; }

    // Case: flagC clear, p2 clear
    o.m_flagC = 0;
    o.m_flagD7 = 99;
    g_h1 = g_h2 = 0;
    CUserProfileManager_SetCameraResetting(&o, 0, 3, 0);
    if (o.m_flagD7 != 99) { std::printf("FAIL d7-2\n"); return 1; }
    if (o.m_flag17 != 3) { std::printf("FAIL 17-2\n"); return 1; }
    if (g_h1 != 1) { std::printf("FAIL h1-2\n"); return 1; }
    if (g_h2 != 0) { std::printf("FAIL h2-2\n"); return 1; }

    std::printf("CUserProfileManager_0040b210_TEST PASS\n");
    return 0;
}