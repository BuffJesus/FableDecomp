#include <stdio.h>

struct CTCMapwho
{
    int tag;
    void PreDie();
    int  OnDie();
};

static int g_predie_calls = 0;
static int g_globalfn_calls = 0;
static CTCMapwho *g_last_arg = 0;

void CTCMapwho::PreDie()
{
    g_predie_calls++;
}

static void __cdecl OnMapwhoDieImpl(CTCMapwho *p)
{
    g_globalfn_calls++;
    g_last_arg = p;
}

static void (__cdecl *g_pOnMapwhoDie)(CTCMapwho *) = OnMapwhoDieImpl;

int CTCMapwho::OnDie()
{
    if (this)
    {
        this->PreDie();
        g_pOnMapwhoDie(this);
    }
    return 0;
}

int main()
{
    CTCMapwho m;
    m.tag = 7;
    int r = m.OnDie();

    bool ok = (r == 0)
           && (g_predie_calls == 1)
           && (g_globalfn_calls == 1)
           && (g_last_arg == &m);

    // null-self path: must not call either
    CTCMapwho *pn = 0;
    // simulate the null branch by not invoking (can't call through null legally),
    // rely on the guarded logic above.

    if (ok)
        printf("MAPWHO_ONDIE_OK\n");
    else
        printf("MAPWHO_ONDIE_FAIL r=%d p=%d g=%d\n", r, g_predie_calls, g_globalfn_calls);

    return ok ? 0 : 1;
}