struct Sub { char pad[0xd8]; unsigned char flag; };
struct CGameScriptInterface { char pad0[0xc]; Sub* sub; };
extern void __fastcall CGameScriptInterface_DoFade(CGameScriptInterface* self);
void __fastcall CGameScriptInterface_FadeScreenIn(CGameScriptInterface* self)
{
    self->sub->flag = 0;
    CGameScriptInterface_DoFade(self);
}