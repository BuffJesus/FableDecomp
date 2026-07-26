struct Sub { void** vtbl; };
struct CScriptThing { void* pad0; Sub* sub; };

typedef bool (__fastcall *SubFn)(Sub* self);

bool __fastcall CScriptThing_MsgPerformedAnyAggressiveSpecialAbility(CScriptThing* self)
{
    Sub* s = self->sub;
    if (!s)
        return false;
    return ((SubFn)(*(void***)s)[0x9c / 4])(s);
}