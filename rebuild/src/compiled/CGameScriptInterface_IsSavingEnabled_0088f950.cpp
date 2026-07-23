struct CInner { char pad[0xd7]; char flag; };
struct CGameScriptInterface { int dummy; CInner* inner; };

char __fastcall CGameScriptInterface_IsSavingEnabled(const CGameScriptInterface* self)
{
    return self->inner->flag;
}