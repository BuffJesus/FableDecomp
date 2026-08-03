
struct Thread { char pad[0x18]; char terminating; };
struct CScriptBase { char pad[0x8]; Thread* activeThread; };

char __fastcall IsActiveThreadTerminating(CScriptBase* self)
{
    Thread* t = self->activeThread;
    if (t)
        return t->terminating;
    return 1;
}