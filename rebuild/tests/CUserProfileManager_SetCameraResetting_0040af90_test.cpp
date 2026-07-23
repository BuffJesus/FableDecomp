#include <cstdio>

struct CUserProfileManager {
    char pad_c[0xc];
    char flag_c;
    char field_d;
    char pad_e[0xcd - 0xe];
    char field_cd;
    void ApplyCamera();
    void NotifyReset();
    void SetCameraResetting(char a1, char a2);
};

static int g_apply = 0;
static int g_notify = 0;

void CUserProfileManager::ApplyCamera() { g_apply++; }
void CUserProfileManager::NotifyReset() { g_notify++; }

void CUserProfileManager::SetCameraResetting(char a1, char a2)
{
    if (this->flag_c)
        this->field_cd = a1;
    this->field_d = a1;
    this->ApplyCamera();
    if (a2)
        this->NotifyReset();
}

int main() {
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); i++) ((char*)&o)[i] = 0;

    o.flag_c = 1;
    g_apply = g_notify = 0;
    o.SetCameraResetting((char)5, (char)1);
    if (o.field_cd != 5 || o.field_d != 5 || g_apply != 1 || g_notify != 1) { std::printf("FAIL case1\n"); return 1; }

    o.flag_c = 0;
    o.field_cd = 99;
    g_apply = g_notify = 0;
    o.SetCameraResetting((char)7, (char)0);
    if (o.field_cd != 99 || o.field_d != 7 || g_apply != 1 || g_notify != 0) { std::printf("FAIL case2\n"); return 1; }

    std::printf("CUserProfileManager_0040af90_TEST PASS\n");
    return 0;
}