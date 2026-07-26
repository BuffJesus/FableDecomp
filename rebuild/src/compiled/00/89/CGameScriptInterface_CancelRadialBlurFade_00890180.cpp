struct CTarget;
extern void __fastcall Callee(CTarget* self);

struct CInner {
    char pad[0x1964];
    CTarget* fld;
};
struct CMid {
    char pad[0x18];
    CInner* inner;
};
struct CGameScriptInterface {
    char pad[4];
    CMid* mid;
};

void __fastcall CGameScriptInterface_CancelRadialBlurFade(CGameScriptInterface* self)
{
    Callee(self->mid->inner->fld);
}