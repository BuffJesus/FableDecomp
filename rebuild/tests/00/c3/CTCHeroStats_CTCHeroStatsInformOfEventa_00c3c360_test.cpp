#include <cstdio>

struct CBase { int tag; };

typedef void (__stdcall *InformOfEvent_fn)(CBase* e);
extern "C" InformOfEvent_fn g_InformOfEventImpl;

struct CTCHeroStats
{
    static void InformOfEvent_a(CBase* e);
};

void CTCHeroStats::InformOfEvent_a(CBase* e)
{
    g_InformOfEventImpl(e);
}

static CBase* g_lastArg = 0;

void __stdcall FakeInformOfEvent(CBase* e)
{
    g_lastArg = e;
}

InformOfEvent_fn g_InformOfEventImpl = FakeInformOfEvent;

int main()
{
    CBase b;
    b.tag = 1234;

    CTCHeroStats::InformOfEvent_a(&b);

    if (g_lastArg == &b && g_lastArg->tag == 1234)
    {
        printf("INFORMOFEVENT_OK\n");
    }
    else
    {
        printf("INFORMOFEVENT_FAIL\n");
    }

    return 0;
}