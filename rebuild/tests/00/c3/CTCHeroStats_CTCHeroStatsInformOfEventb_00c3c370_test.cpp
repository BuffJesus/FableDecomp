#include <cstdio>

struct CBase { int tag; };

extern "C" void (__stdcall *g_HeroStatsEventNotify)(CBase* e) = 0;

extern "C" void __cdecl CTCHeroStats_InformOfEvent_b(CBase* e)
{
    g_HeroStatsEventNotify(e);
}

static CBase g_lastArg;
static bool g_called = false;

void __stdcall FakeNotify(CBase* e)
{
    g_called = true;
    g_lastArg = *e;
}

int main()
{
    g_HeroStatsEventNotify = FakeNotify;
    CBase ev; ev.tag = 42;
    CTCHeroStats_InformOfEvent_b(&ev);
    if (g_called && g_lastArg.tag == 42) {
        printf("INFORMOFEVENT_B_OK\n");
    } else {
        printf("INFORMOFEVENT_B_FAIL\n");
    }
    return 0;
}