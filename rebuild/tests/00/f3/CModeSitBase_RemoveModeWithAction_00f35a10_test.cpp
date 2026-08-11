#include <stdio.h>

struct CCreatureActionBase {
    virtual void slot0() = 0;
    virtual int  slot1() = 0;
};

struct ModeContainer2C {
    void RemoveMode(int actionId);
};

struct CModeSitBase {
    char pad[0x2c];
    ModeContainer2C container;
    void RemoveModeWithAction(CCreatureActionBase* action);
};

void CModeSitBase::RemoveModeWithAction(CCreatureActionBase* action)
{
    this->container.RemoveMode( action->slot1() );
}

static int g_removed = -1;
void ModeContainer2C::RemoveMode(int actionId) { g_removed = actionId; }

struct TestAction : CCreatureActionBase {
    int id;
    void slot0() {}
    int  slot1() { return id; }
};

int main()
{
    CModeSitBase m;
    TestAction a;
    a.id = 4242;
    m.RemoveModeWithAction(&a);
    if (g_removed == 4242) {
        printf("SIT_REMOVE_OK\n");
        return 0;
    }
    printf("FAIL %d\n", g_removed);
    return 1;
}