#include <cstdio>

struct CCreatureActionBase
{
    virtual int v0() = 0;
    virtual int v1() = 0;
    virtual int v2() = 0;
    virtual int v3() = 0;
    virtual int v4() = 0;
    virtual int v5() = 0;
    virtual int v6() = 0;
    virtual int v7() = 0;
    virtual int v8() = 0;
    virtual int v9() = 0;
    virtual int v10() = 0;
    virtual int GetActionType() = 0;
};

struct CModeList
{
    void Remove(int key);
    int junk;
};

struct CModeSitBase
{
    char pad[0x2c];
    CModeList list;
    void RemoveModeWithAction(CCreatureActionBase* action);
};

void CModeSitBase::RemoveModeWithAction(CCreatureActionBase* action)
{
    this->list.Remove(action->GetActionType());
}

// --- test doubles ---
static int g_removed_key = -999;

void CModeList::Remove(int key)
{
    g_removed_key = key + this->junk;
}

struct TestAction : public CCreatureActionBase
{
    int v0()  { return 0; }
    int v1()  { return 0; }
    int v2()  { return 0; }
    int v3()  { return 0; }
    int v4()  { return 0; }
    int v5()  { return 0; }
    int v6()  { return 0; }
    int v7()  { return 0; }
    int v8()  { return 0; }
    int v9()  { return 0; }
    int v10() { return 0; }
    int GetActionType() { return 4242; }
};

int main()
{
    CModeSitBase self;
    self.list.junk = 0;
    TestAction act;
    self.RemoveModeWithAction(&act);
    if (g_removed_key == 4242)
        printf("REMOVEMODE_OK\n");
    else
        printf("REMOVEMODE_FAIL %d\n", g_removed_key);
    return (g_removed_key == 4242) ? 0 : 1;
}