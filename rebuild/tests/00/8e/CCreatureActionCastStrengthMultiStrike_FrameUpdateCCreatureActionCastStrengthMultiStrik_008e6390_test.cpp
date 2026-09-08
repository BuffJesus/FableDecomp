#include <cstdio>
#include "engine/CCreatureAction_CastStrengthMultiStrike.h"

static int g_base_called;
static int g_should_ret;
static int g_strike_called;

void __fastcall CCreatureAction_Base_FrameUpdate(CCreatureAction_CastStrengthMultiStrike*) {
    ++g_base_called;
}

struct CCreatureAction_CastStrengthMultiStrike_Methods : CCreatureAction_CastStrengthMultiStrike {
    virtual void v0(){} virtual void v1(){} virtual void v2(){} virtual void v3(){}
    virtual void DoStrike(){ ++g_strike_called; }
    virtual void v5(){} virtual void v6(){} virtual void v7(){} virtual void v8(){} virtual void v9(){}
    virtual void v10(){} virtual void v11(){} virtual void v12(){} virtual void v13(){} virtual void v14(){}
    virtual void v15(){} virtual void v16(){} virtual void v17(){} virtual void v18(){} virtual void v19(){}
    virtual void v20(){} virtual void v21(){} virtual void v22(){} virtual void v23(){} virtual void v24(){}
    virtual void v25(){} virtual void v26(){}
    virtual bool ShouldStrike(){ return g_should_ret != 0; }
    virtual void FrameUpdate();
};

int main() {
    CCreatureAction_CastStrengthMultiStrike_Methods action;
    g_should_ret = 0;
    action.FrameUpdate();
    bool falseBranch = g_base_called == 1 && g_strike_called == 0;

    g_base_called = g_strike_called = 0;
    g_should_ret = 1;
    action.FrameUpdate();
    bool trueBranch = g_base_called == 1 && g_strike_called == 1;

    if (falseBranch && trueBranch) {
        std::printf("MULTISTRIKE_FRAMEUPDATE_OK\n");
        return 0;
    }
    return 1;
}
