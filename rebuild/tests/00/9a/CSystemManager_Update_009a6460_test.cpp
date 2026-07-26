#include <stdio.h>

struct CSystemManagerObserver
{
    virtual void Placeholder0() = 0;
    virtual void Placeholder1() = 0;
    virtual void Placeholder2() = 0;
    virtual void OnSystemUpdate() = 0;
};

enum ESystemReturn
{
    FABLE_SYSTEM_CONTINUE = 1,
    FABLE_SYSTEM_BUSY = 2
};

struct CSystemManager
{
    unsigned char unknown00[0x08];
    bool busy08;
    bool notifyObserver09;
    unsigned char unknown0A[0x72];
    CSystemManagerObserver* observer7C;
    unsigned char unknown80[0x6A];
    bool resetPendingEA;

    void UpdateActive();
    ESystemReturn Update();
};

static int g_updateActiveCalls = 0;

void CSystemManager::UpdateActive()
{
    ++g_updateActiveCalls;
}

struct TestObserver : CSystemManagerObserver
{
    int updateCalls;

    TestObserver() : updateCalls(0)
    {
    }

    virtual void Placeholder0()
    {
    }

    virtual void Placeholder1()
    {
    }

    virtual void Placeholder2()
    {
    }

    virtual void OnSystemUpdate()
    {
        ++updateCalls;
    }
};

int main()
{
    TestObserver observer;
    CSystemManager manager = {};
    manager.notifyObserver09 = true;
    manager.observer7C = &observer;
    manager.resetPendingEA = true;

    if (manager.Update() != FABLE_SYSTEM_CONTINUE ||
        manager.resetPendingEA ||
        observer.updateCalls != 1 ||
        g_updateActiveCalls != 1)
    {
        printf("FABLETLC_SYSTEM_MANAGER_UPDATE_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    manager.busy08 = true;
    manager.resetPendingEA = true;
    if (manager.Update() != FABLE_SYSTEM_BUSY ||
        !manager.resetPendingEA ||
        observer.updateCalls != 1 ||
        g_updateActiveCalls != 2)
    {
        printf("FABLETLC_SYSTEM_MANAGER_UPDATE_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_SYSTEM_MANAGER_UPDATE_BEHAVIOR PASS\n");
    return 0;
}
