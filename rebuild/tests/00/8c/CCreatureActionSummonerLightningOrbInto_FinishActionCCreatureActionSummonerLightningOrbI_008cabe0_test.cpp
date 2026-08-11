#include <cstdio>

struct CCreatureAction_SummonerLightningOrbInto
{
    virtual void vf0();
    virtual void vf1();
    virtual void vf2();
    virtual void OnFinishHook();
    bool Check();
    void FinishAction();
};

static int g_hook = 0;
static int g_base = 0;
static bool g_checkResult = false;

bool CCreatureAction_SummonerLightningOrbInto::Check() { return g_checkResult; }
void CCreatureAction_SummonerLightningOrbInto::vf0() {}
void CCreatureAction_SummonerLightningOrbInto::vf1() {}
void CCreatureAction_SummonerLightningOrbInto::vf2() {}
void CCreatureAction_SummonerLightningOrbInto::OnFinishHook() { g_hook++; }
void BaseFinishAction(CCreatureAction_SummonerLightningOrbInto*) { g_base++; }

void CCreatureAction_SummonerLightningOrbInto::FinishAction()
{
    if (!this->Check())
        this->OnFinishHook();
    BaseFinishAction(this);
}

int main()
{
    CCreatureAction_SummonerLightningOrbInto obj;

    g_hook = g_base = 0;
    g_checkResult = false;   // check false -> hook runs
    obj.FinishAction();
    bool ok1 = (g_hook == 1 && g_base == 1);

    g_hook = g_base = 0;
    g_checkResult = true;    // check true -> hook skipped
    obj.FinishAction();
    bool ok2 = (g_hook == 0 && g_base == 1);

    if (ok1 && ok2) { printf("FINISHACTION_ORB_OK\n"); return 0; }
    printf("FAIL\n");
    return 1;
}