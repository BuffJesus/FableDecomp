#include <cstdio>

struct CScriptThing;

static int g_slot_called = 0;
static CScriptThing* g_slot_arg = 0;
static int g_null_called = 0;
static CScriptThing* g_null_killer = 0;

struct CInner {
    virtual void v00(){} virtual void v01(){} virtual void v02(){} virtual void v03(){}
    virtual void v04(){} virtual void v05(){} virtual void v06(){} virtual void v07(){}
    virtual void v08(){} virtual void v09(){} virtual void v10(){} virtual void v11(){}
    virtual void v12(){} virtual void v13(){} virtual void v14(){} virtual void v15(){}
    virtual void v16(){} virtual void v17(){} virtual void v18(){}
    virtual void OnKilled(CScriptThing* msg);
};

struct CScriptThing {
    unsigned char _pad_0x0[0x4];
    CInner* inner;
    CScriptThing* MsgWhoKilledMe(CScriptThing* msg);
    void HandleNull(CScriptThing* killer);
};

void CInner::OnKilled(CScriptThing* msg)
{
    g_slot_called = 1;
    g_slot_arg = msg;
}

void CScriptThing::HandleNull(CScriptThing* killer)
{
    g_null_called = 1;
    g_null_killer = killer;
}

CScriptThing* CScriptThing::MsgWhoKilledMe(CScriptThing* msg)
{
    CInner* p = this->inner;
    if (p == 0) {
        msg->HandleNull(this);
        return msg;
    }
    p->OnKilled(msg);
    return msg;
}

int main()
{
    // Case 1: inner == null
    CScriptThing a; a.inner = 0;
    CScriptThing msg1;
    g_null_called = 0; g_null_killer = 0;
    CScriptThing* r1 = a.MsgWhoKilledMe(&msg1);
    if (r1 != &msg1 || g_null_called != 1 || g_null_killer != &a) {
        std::printf("FAIL case1\n");
        return 1;
    }

    // Case 2: inner != null -> virtual OnKilled(msg)
    CInner innr;
    CScriptThing b; b.inner = &innr;
    CScriptThing msg2;
    g_slot_called = 0; g_slot_arg = 0;
    CScriptThing* r2 = b.MsgWhoKilledMe(&msg2);
    if (r2 != &msg2 || g_slot_called != 1 || g_slot_arg != &msg2) {
        std::printf("FAIL case2\n");
        return 1;
    }

    std::printf("CScriptThing_004abe60_TEST PASS\n");
    return 0;
}