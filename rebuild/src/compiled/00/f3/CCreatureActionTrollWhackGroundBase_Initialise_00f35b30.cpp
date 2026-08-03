struct Thread { char pad[0x18]; char terminating; };
struct CScriptBase { char pad[0x8]; Thread* activeThread; };

__declspec(noinline) char __fastcall IsActiveThreadTerminating(CScriptBase* self)
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