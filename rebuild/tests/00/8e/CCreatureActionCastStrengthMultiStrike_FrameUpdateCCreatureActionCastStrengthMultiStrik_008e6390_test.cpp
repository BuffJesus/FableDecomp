#include <cstdio>

struct CCreatureAction_CastStrengthMultiStrike;

static int g_base_called = 0;
static int g_should_ret = 0;
static int g_strike_called = 0;

// definition of the external base call the source references
void __fastcall CCreatureAction_Base_FrameUpdate(CCreatureAction_CastStrengthMultiStrike*)
{
    g_base_called++;
}

// mirror the source's class layout
struct CCreatureAction_CastStrengthMultiStrike
{
    virtual void v0(){}
    virtual void v1(){}
    virtual void v2(){}
    virtual void v3(){}
    virtual void DoStrike(){ g_strike_called++; }
    virtual void v5(){}
    virtual void v6(){}
    virtual void v7(){}
    virtual void v8(){}
    virtual void v9(){}
    virtual void v10(){}
    virtual void v11(){}
    virtual void v12(){}
    virtual void v13(){}
    virtual void v14(){}
    virtual void v15(){}
    virtual void v16(){}
    virtual void v17(){}
    virtual void v18(){}
    virtual void v19(){}
    virtual void v20(){}
    virtual void v21(){}
    virtual void v22(){}
    virtual void v23(){}
    virtual void v24(){}
    virtual void v25(){}
    virtual void v26(){}
    virtual bool ShouldStrike(){ return g_should_ret != 0; }
    virtual void FrameUpdate();
};

int main()
{
    CCreatureAction_CastStrengthMultiStrike obj;

    // Case 1: ShouldStrike returns false -> base called, no strike
    g_base_called = g_strike_called = 0;
    g_should_ret = 0;
    obj.FrameUpdate();
    bool ok1 = (g_base_called == 1) && (g_strike_called == 0);

    // Case 2: ShouldStrike returns true -> base called AND strike
    g_base_called = g_strike_called = 0;
    g_should_ret = 1;
    obj.FrameUpdate();
    bool ok2 = (g_base_called == 1) && (g_strike_called == 1);

    if (ok1 && ok2) {
        printf("MULTISTRIKE_FRAMEUPDATE_OK\n");
        return 0;
    }
    printf("FAIL c1=%d c2=%d\n", (int)ok1, (int)ok2);
    return 1;
}