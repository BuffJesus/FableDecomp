#include <cstdio>

struct CUserProfileManager {
    char pad0[0x0c];
    char flag0c;
    char pad1[0x13];
    int  val20;
    char pad2[0xbc];
    int  vale0;
};

int g_apply = 0;
int g_refresh = 0;

void __fastcall CUserProfileManager_Apply(CUserProfileManager* self) { (void)self; g_apply++; }
void __fastcall CUserProfileManager_Refresh(CUserProfileManager* self) { (void)self; g_refresh++; }

void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, int edx, volatile int val, char flag)
{
    if (self->flag0c)
        self->vale0 = val;
    self->val20 = val;
    CUserProfileManager_Apply(self);
    if (flag)
        CUserProfileManager_Refresh(self);
}

int main()
{
    CUserProfileManager m;
    for (int i = 0; i < (int)sizeof(m); ++i) ((char*)&m)[i] = 0;

    // Case 1: flag0c set, flag arg set
    m.flag0c = 1;
    CUserProfileManager_SetAntialiasing(&m, 0, 1234, 1);
    if (m.vale0 != 1234) { std::printf("FAIL vale0\n"); return 1; }
    if (m.val20 != 1234) { std::printf("FAIL val20\n"); return 1; }
    if (g_apply != 1) { std::printf("FAIL apply\n"); return 1; }
    if (g_refresh != 1) { std::printf("FAIL refresh\n"); return 1; }

    // Case 2: flag0c clear, flag arg clear
    m.flag0c = 0;
    m.vale0 = 999;
    g_apply = 0; g_refresh = 0;
    CUserProfileManager_SetAntialiasing(&m, 0, 55, 0);
    if (m.vale0 != 999) { std::printf("FAIL vale0 2\n"); return 1; }
    if (m.val20 != 55) { std::printf("FAIL val20 2\n"); return 1; }
    if (g_apply != 1) { std::printf("FAIL apply 2\n"); return 1; }
    if (g_refresh != 0) { std::printf("FAIL refresh 2\n"); return 1; }

    std::printf("CUserProfileManager_0040b310_TEST PASS\n");
    return 0;
}