#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char flag_c;                 /* +0x0c */
    char pad1[0x19 - 0x0c - 1];
    char at19;                   /* +0x19 */
    char pad2[0xd9 - 0x19 - 1];
    char atD9;                   /* +0xd9 */
    void Helper1();
    void Helper2();
    void SetCameraResetting(char a, char b);
};

static int g_h1 = 0;
static int g_h2 = 0;

void CUserProfileManager::Helper1() { g_h1++; }
void CUserProfileManager::Helper2() { g_h2++; }

void CUserProfileManager::SetCameraResetting(char a, char b)
{
    if (this->flag_c)
        this->atD9 = a;
    this->at19 = a;
    this->Helper1();
    if (b)
        this->Helper2();
}

int main()
{
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); i++) ((char*)&o)[i] = 0;

    o.flag_c = 1;
    o.SetCameraResetting((char)7, (char)1);
    if (o.atD9 != 7) { std::printf("FAIL atD9\n"); return 1; }
    if (o.at19 != 7) { std::printf("FAIL at19\n"); return 1; }
    if (g_h1 != 1)   { std::printf("FAIL h1\n"); return 1; }
    if (g_h2 != 1)   { std::printf("FAIL h2\n"); return 1; }

    o.flag_c = 0; o.atD9 = 55; o.at19 = 0; g_h1 = 0; g_h2 = 0;
    o.SetCameraResetting((char)9, (char)0);
    if (o.atD9 != 55) { std::printf("FAIL atD9 preserved\n"); return 1; }
    if (o.at19 != 9)  { std::printf("FAIL at19b\n"); return 1; }
    if (g_h1 != 1)    { std::printf("FAIL h1b\n"); return 1; }
    if (g_h2 != 0)    { std::printf("FAIL h2b\n"); return 1; }

    std::printf("CUserProfileManager_0040b290_TEST PASS\n");
    return 0;
}