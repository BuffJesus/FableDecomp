// Byte-exact reconstruction of IsThingAwareOfHero @ 0x00cbe2e0
// __fastcall(CScriptThing* self) -> bool
// return Check() && (*(this->vtbl)[0x40/4])();

struct CScriptThing;

// vtable modeled as array of function pointers; slot 0x40 = index 16
typedef char (__fastcall *VFn)(CScriptThing* self);

struct CScriptThing {
    VFn* vtbl;   // [esi] -> vtable at offset 0
};

// direct (non-virtual) member helper at 0xcbe1aa
extern char __fastcall CST_HelperCheck(CScriptThing* self);

bool __fastcall IsThingAwareOfHero(CScriptThing* self)
{
    if (CST_HelperCheck(self))
    {
        if (self->vtbl[0x10](self))   // [eax+0x40] = index 16
            return true;
    }
    return false;
}