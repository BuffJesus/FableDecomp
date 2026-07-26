struct CScriptThing;

struct CScriptThingVtbl {
    unsigned char _pad[0x12c];
    unsigned char (__fastcall *IsAlive)(CScriptThing* self);
};

struct CScriptThing {
    CScriptThingVtbl* vtbl;
};

bool __fastcall CScriptThing_IsDead(CScriptThing* self)
{
    return self->vtbl->IsAlive(self) == 0;
}