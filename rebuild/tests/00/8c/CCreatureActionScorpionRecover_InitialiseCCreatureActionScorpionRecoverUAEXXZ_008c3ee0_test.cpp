#include <cstdio>

// Independent behaviour test mirroring the same shape.

static int g_baseInitCalls = 0;
static int g_getTargetCalls = 0;
static int g_slot70Flag = -1;

struct Target {
    virtual void v00() {}
    virtual void v04() {}
    virtual void v08() {}
    virtual void v0c() {}
    virtual void v10() {}
    virtual void v14() {}
    virtual void v18() {}
    virtual void v1c() {}
    virtual void v20() {}
    virtual void v24() {}
    virtual void v28() {}
    virtual void v2c() {}
    virtual void v30() {}
    virtual void v34() {}
    virtual void v38() {}
    virtual void v3c() {}
    virtual void v40() {}
    virtual void v44() {}
    virtual void v48() {}
    virtual void v4c() {}
    virtual void v50() {}
    virtual void v54() {}
    virtual void v58() {}
    virtual void v5c() {}
    virtual void v60() {}
    virtual void v64() {}
    virtual void v68() {}
    virtual void v6c() {}
    virtual void slot70(int flag) { g_slot70Flag = flag; }
};

static Target g_target;

struct Member8 {
    Target* GetTarget() { g_getTargetCalls++; return &g_target; }
};

struct CCreatureAction_ScorpionRecover {
    void BaseInit() { g_baseInitCalls++; }
    char pad[4];
    Member8 m8;
    virtual void Initialise();
};

void CCreatureAction_ScorpionRecover::Initialise()
{
    BaseInit();
    Target* t = m8.GetTarget();
    t->slot70(1);
}

int main()
{
    CCreatureAction_ScorpionRecover a;
    a.Initialise();
    if (g_baseInitCalls == 1 && g_getTargetCalls == 1 && g_slot70Flag == 1) {
        printf("SCORPIONRECOVER_INIT_OK\n");
        return 0;
    }
    printf("FAIL b=%d g=%d f=%d\n", g_baseInitCalls, g_getTargetCalls, g_slot70Flag);
    return 1;
}