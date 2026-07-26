struct CMid {
    void Save(int a);
};

struct CGameScriptInterface {
    void* vt;
    CMid* mid;
};

void __fastcall CGameScriptInterface_AutoSave(CGameScriptInterface* self)
{
    self->mid->Save(0);
}