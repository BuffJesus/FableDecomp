#include <cstdio>

struct CManagerFwd;

struct IErrorMessageSink
{
    virtual void f0() = 0;
    virtual void f1() = 0;
    virtual void f2() = 0;
    virtual void UpdateErrorMessage(CManagerFwd* pOwner) = 0;
};

struct CManagerFwd
{
    void* vtbl;
    IErrorMessageSink* sink;
};

typedef CManagerFwd CManager;

void __fastcall CManager_UpdateErrorMessage_a(CManager* self, void* /*edx*/)
{
    self->sink->UpdateErrorMessage(self);
}

static int g_calls = 0;
static void* g_lastSelf = 0;

struct SinkImpl : IErrorMessageSink
{
    virtual void f0() {}
    virtual void f1() {}
    virtual void f2() {}
    virtual void UpdateErrorMessage(CManagerFwd* pOwner)
    {
        g_calls = g_calls + 1;
        g_lastSelf = pOwner;
    }
};

int main()
{
    SinkImpl sink;
    CManager mgr;
    mgr.vtbl = 0;
    mgr.sink = &sink;

    CManager_UpdateErrorMessage_a(&mgr, 0);

    if (g_calls == 1 && g_lastSelf == (void*)&mgr)
    {
        printf("PASS_UPDATE_ERROR_MSG_FORWARD\n");
    }
    else
    {
        printf("FAIL\n");
    }

    return 0;
}