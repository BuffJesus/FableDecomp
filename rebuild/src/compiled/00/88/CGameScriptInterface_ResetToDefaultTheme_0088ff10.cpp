struct CInner {
    void Method(int a, int b);
};
struct CMid {
    char pad[0x18];
    CInner* p18;
};
struct CGameScriptInterface {
    char pad0[4];
    CMid* p4;
};

void __fastcall CGameScriptInterface_ResetToDefaultTheme(CGameScriptInterface* self, int edx, int arg)
{
    self->p4->p18->Method(1, arg);
}