struct CThing;

struct CThingRef {
    CThing* Get();
};

struct CGameScriptThing {
    char pad[0xc];
    CThingRef ref;
};

CThing* __fastcall CGameScriptThing_GetPThing(const CGameScriptThing* self)
{
    CThingRef* r = (CThingRef*)&self->ref;
    if (r->Get() != 0) {
        if ((*((unsigned char*)r->Get() + 0x91) & 1) == 0)
            return r->Get();
    }
    return 0;
}