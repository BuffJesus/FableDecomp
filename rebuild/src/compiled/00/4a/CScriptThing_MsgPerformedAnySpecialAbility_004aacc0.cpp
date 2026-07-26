struct Target;
struct TargetVtbl {
    char pad[0x98];
    bool (__fastcall *slot)(Target*);
};
struct Target {
    TargetVtbl* vtbl;
};
struct CScriptThing {
    void* pad0;
    Target* m_target;
};

bool __fastcall CScriptThing_MsgPerformedAnySpecialAbility(CScriptThing* self)
{
    Target* t = self->m_target;
    if (t == 0)
        return false;
    return t->vtbl->slot(t);
}