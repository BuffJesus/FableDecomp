#include <cstdio>

struct Sub {
    char pad[4];
};

static int g_calls = 0;
static Sub* g_last[2];

void __fastcall HelperRelocate(Sub* self)
{
    if (g_calls < 2) g_last[g_calls] = self;
    g_calls++;
}

struct CLandscapeBackgroundSubNodeSplitBranch {
    Sub sub0;
    char pad1[0xe4];
    Sub sub1;
    virtual void OnPostRelocation();
};

void CLandscapeBackgroundSubNodeSplitBranch::OnPostRelocation()
{
    HelperRelocate(&this->sub0);
    HelperRelocate(&this->sub1);
}

int main()
{
    CLandscapeBackgroundSubNodeSplitBranch obj;
    char* base = (char*)&obj;
    obj.OnPostRelocation();

    bool ok = (g_calls == 2)
        && ((char*)g_last[0] == base + 0x04)
        && ((char*)g_last[1] == base + 0xec);

    if (ok) printf("00bf6d10_TEST PASS\n");
    else printf("FAIL calls=%d o0=%ld o1=%ld\n", g_calls,
                (long)((char*)g_last[0]-base), (long)((char*)g_last[1]-base));
    return 0;
}