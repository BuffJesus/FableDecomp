struct CBase;

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