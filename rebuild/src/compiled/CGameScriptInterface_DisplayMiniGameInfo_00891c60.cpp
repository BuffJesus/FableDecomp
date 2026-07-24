struct Target {
    void Show(int arg1, int arg2, int zero);
};
struct Mgr {
    unsigned char _pad[0x1c8];
    Target* target; // +0x1c8
};
extern Mgr* g_mgr;

void __stdcall CGameScriptInterface_DisplayMiniGameInfo(int arg1, int arg2)
{
    g_mgr->target->Show(arg1, arg2, 0);
}