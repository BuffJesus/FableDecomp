struct CInner { void EndLetterBox(); };
struct CGameScriptInterface { char pad[0xc]; CInner* inner; };
extern "C" void __fastcall CGameScriptInterface_EndLetterBox(CGameScriptInterface* self)
{
    self->inner->EndLetterBox();
}