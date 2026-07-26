#include <cstdio>
struct CUserProfileManager { char pad[0x33]; char flag; };
static int g_prep = 0;
static int g_commit = 0;
extern "C" void __fastcall UPM_Prep(CUserProfileManager* self) { (void)self; g_prep++; }
extern "C" void __fastcall UPM_Commit(CUserProfileManager* self) { (void)self; g_commit++; }

void __fastcall CUserProfileManager_Apply(CUserProfileManager* self)
{
    UPM_Prep(self);
    if (self->flag)
        UPM_Commit(self);
}

int main()
{
    CUserProfileManager a;
    a.flag = 1;
    g_prep = g_commit = 0;
    CUserProfileManager_Apply(&a);
    if (g_prep != 1 || g_commit != 1) { std::printf("FAIL branch-taken prep=%d commit=%d\n", g_prep, g_commit); return 1; }

    CUserProfileManager b;
    b.flag = 0;
    g_prep = g_commit = 0;
    CUserProfileManager_Apply(&b);
    if (g_prep != 1 || g_commit != 0) { std::printf("FAIL branch-skip prep=%d commit=%d\n", g_prep, g_commit); return 1; }

    std::printf("CUserProfileManager_0040bc80_TEST PASS\n");
    return 0;
}