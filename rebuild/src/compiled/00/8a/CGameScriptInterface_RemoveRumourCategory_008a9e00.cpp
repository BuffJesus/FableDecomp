struct Cat {
    int x;
    void ctor(int arg);
    void dtor();
};
struct Mgr {
    void remove(Cat* c);
};

void __fastcall CGameScriptInterface_RemoveRumourCategory(void* self, int edx, int arg)
{
    Cat local;
    local.ctor(arg);
    ((Mgr*)0x13bae44)->remove(&local);
    local.dtor();
}