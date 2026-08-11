struct CScriptBase;

struct CScriptBaseVtbl {
    void (__fastcall *slot0)(CScriptBase*);
    void (__fastcall *slot1)(CScriptBase*);
    void (__fastcall *slot2)(CScriptBase*);
    void (__fastcall *slot3)(CScriptBase*);
};

struct CScriptBase {
    CScriptBaseVtbl* vtbl;
};

void __fastcall Activate(CScriptBase* self)
{
    self->vtbl->slot3(self);
    self->vtbl->slot1(self);
}