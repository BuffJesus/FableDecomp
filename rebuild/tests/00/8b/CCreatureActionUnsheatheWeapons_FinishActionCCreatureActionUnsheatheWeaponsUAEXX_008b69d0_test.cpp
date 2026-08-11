#include <stdio.h>

struct CCreatureAction_UnsheatheWeapons;

bool __fastcall UnsheatheWeapons_CheckPredicate(CCreatureAction_UnsheatheWeapons* self);
void __fastcall UnsheatheWeapons_BaseFinishAction(CCreatureAction_UnsheatheWeapons* self);

struct CCreatureAction_UnsheatheWeapons
{
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void OnFinishHook() = 0;
    virtual void FinishAction() = 0;
};

// ---- test state ----
static int g_predicateResult = 0;
static int g_hookCalled = 0;
static int g_baseCalled = 0;

// external member helpers used by s.cpp
bool __fastcall UnsheatheWeapons_CheckPredicate(CCreatureAction_UnsheatheWeapons*)
{
    return g_predicateResult != 0;
}
void __fastcall UnsheatheWeapons_BaseFinishAction(CCreatureAction_UnsheatheWeapons*)
{
    g_baseCalled++;
}

// concrete subclass to provide vtable bodies
struct TestAction : public CCreatureAction_UnsheatheWeapons
{
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void OnFinishHook() { g_hookCalled++; }
    virtual void FinishAction();
};

// independent replica of the reconstructed body (standalone test)
void TestAction::FinishAction()
{
    if (!UnsheatheWeapons_CheckPredicate(this))
        this->OnFinishHook();
    UnsheatheWeapons_BaseFinishAction(this);
}

int main()
{
    // case 1: predicate false -> hook fires, base fires
    {
        TestAction a;
        g_predicateResult = 0; g_hookCalled = 0; g_baseCalled = 0;
        a.FinishAction();
        if (g_hookCalled != 1 || g_baseCalled != 1)
        {
            printf("FAIL case1 hook=%d base=%d\n", g_hookCalled, g_baseCalled);
            return 1;
        }
    }
    // case 2: predicate true -> hook skipped, base still fires
    {
        TestAction a;
        g_predicateResult = 1; g_hookCalled = 0; g_baseCalled = 0;
        a.FinishAction();
        if (g_hookCalled != 0 || g_baseCalled != 1)
        {
            printf("FAIL case2 hook=%d base=%d\n", g_hookCalled, g_baseCalled);
            return 1;
        }
    }
    printf("UNSHEATHE_FINISH_OK\n");
    return 0;
}