#include <cstdio>

struct CUserProfileManager {
    char pad_0[0xc];
    char m_flag;      /* +0xc */
    char pad_d[3];    /* +0xd..0xf */
    char m_field10;   /* +0x10 */
    char pad_11[0xbf];/* +0x11..0xcf */
    char m_fieldD0;   /* +0xd0 */
    void Notify();
    void Apply();
    void SetCameraResetting(char value, char doApply);
};

static int g_notify = 0;
static int g_apply = 0;

void CUserProfileManager::Notify() { ++g_notify; }
void CUserProfileManager::Apply()  { ++g_apply; }

void CUserProfileManager::SetCameraResetting(char value, char doApply)
{
    if (this->m_flag != 0)
        this->m_fieldD0 = value;
    this->m_field10 = value;
    this->Notify();
    if (doApply != 0)
        this->Apply();
}

int main()
{
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); ++i) ((char*)&o)[i] = 0;

    o.m_flag = 0;
    o.SetCameraResetting(5, 0);
    if (o.m_field10 != 5 || o.m_fieldD0 != 0 || g_notify != 1 || g_apply != 0) {
        std::printf("FAIL case1\n"); return 1;
    }

    o.m_flag = 1;
    o.SetCameraResetting(7, 1);
    if (o.m_field10 != 7 || o.m_fieldD0 != 7 || g_notify != 2 || g_apply != 1) {
        std::printf("FAIL case2\n"); return 1;
    }

    std::printf("CUserProfileManager_0040b050_TEST PASS\n");
    return 0;
}