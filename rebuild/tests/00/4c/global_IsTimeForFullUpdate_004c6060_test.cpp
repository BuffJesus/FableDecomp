#include <cstdio>

static int g_div = 3;
static int g_num = 9;

extern "C" int __fastcall sub_4c5fa0(void*) { return 0; }
extern "C" int __cdecl   sub_bfea70(void) { return g_div; }
extern "C" int __cdecl   sub_49d870(void) { return g_num; }

struct CBulletTimeManager;
extern "C" bool __fastcall IsTimeForFullUpdate(CBulletTimeManager* self);

int main()
{
    g_div = 3; g_num = 9;
    bool a = IsTimeForFullUpdate((CBulletTimeManager*)0);   // 9%3==0 -> true
    g_div = 4; g_num = 9;
    bool b = IsTimeForFullUpdate((CBulletTimeManager*)0);   // 9%4==1 -> false
    if (a && !b) printf("PARITY_OK\n");
    else printf("PARITY_FAIL a=%d b=%d\n", (int)a, (int)b);
    return 0;
}