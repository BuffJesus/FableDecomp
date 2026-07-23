struct CInner;
struct CInnerVtbl { char pad[0xf0]; bool (__fastcall *IsKicked)(CInner*); };
struct CInner { CInnerVtbl* vtbl; };
struct CScriptThing { void* pad0; CInner* inner; };

bool __fastcall CScriptThing_MsgIsKicked(CScriptThing* self)
{
    CInner* p = self->inner;
    if (!p)
        return false;
    return p->vtbl->IsKicked(p);
}