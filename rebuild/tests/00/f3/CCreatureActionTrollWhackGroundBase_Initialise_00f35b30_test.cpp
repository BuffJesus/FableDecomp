#include <cstdio>

struct Thread { char pad[0x18]; char terminating; };
struct CScriptBase { char pad[0x8]; Thread* activeThread; };

char __fastcall IsActiveThreadTerminating(CScriptBase* self)
{
    Thread* t = self->activeThread;
    if (t)
        return t->terminating;
    return 1;
}

struct CCreatureAction_TrollWhackGroundBase
{
    struct VTable
    {
        void* slot0;
        void* slot1;
        void* slot2;
        CScriptBase* (__fastcall *GetScriptBase)(CCreatureAction_TrollWhackGroundBase*);
    };
    VTable* vtbl;
};

char __fastcall Initialise(CCreatureAction_TrollWhackGroundBase* self)
{
    CScriptBase* sb = self->vtbl->GetScriptBase(self);
    return IsActiveThreadTerminating(sb);
}

static CScriptBase g_sb;
static Thread g_thread;

static CScriptBase* __fastcall FakeGetScriptBase_impl(CCreatureAction_TrollWhackGroundBase* self)
{
    (void)self;
    return &g_sb;
}

int main()
{
    CCreatureAction_TrollWhackGroundBase::VTable vt;
    vt.slot0 = 0;
    vt.slot1 = 0;
    vt.slot2 = 0;
    vt.GetScriptBase = &FakeGetScriptBase_impl;

    CCreatureAction_TrollWhackGroundBase obj;
    obj.vtbl = &vt;

    g_thread.terminating = 1;
    g_sb.activeThread = &g_thread;

    char result = Initialise(&obj);
    if (result == 1)
    {
        printf("TROLLWHACK_INIT_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}