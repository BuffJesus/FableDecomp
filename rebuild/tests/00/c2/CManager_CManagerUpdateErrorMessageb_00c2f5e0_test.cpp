#include <cstdio>

struct IErrTargetB {
    virtual void Slot0() {}
    virtual void Slot1() {}
    virtual void Slot2() {}
    virtual void OnUpdateError(void* arg) { (void)arg; }
};

struct CManagerB_Self {
    void* field0;
    IErrTargetB* pTarget;
};

void __fastcall CManager_UpdateErrorMessage_b(CManagerB_Self* self) {
    self->pTarget->OnUpdateError(self);
}

static void* g_lastSelf = 0;
static void* g_lastArg = 0;
static int g_callCount = 0;

struct FakeErrTargetB : IErrTargetB {
    virtual void Slot0() {}
    virtual void Slot1() {}
    virtual void Slot2() {}
    virtual void OnUpdateError(void* arg) {
        g_lastSelf = this;
        g_lastArg = arg;
        g_callCount++;
    }
};

int main() {
    FakeErrTargetB fakeTarget;
    IErrTargetB* pTargetAddr = &fakeTarget;

    CManagerB_Self obj;
    obj.field0 = 0;
    obj.pTarget = pTargetAddr;

    CManagerB_Self* pObjAddr = &obj;
    CManager_UpdateErrorMessage_b(pObjAddr);

    if (g_callCount == 1 && g_lastSelf == (void*)pTargetAddr && g_lastArg == (void*)pObjAddr) {
        printf("PASS_MARKER_UEM_B\n");
    } else {
        printf("FAIL count=%d self=%p arg=%p\n", g_callCount, g_lastSelf, g_lastArg);
    }
    return 0;
}