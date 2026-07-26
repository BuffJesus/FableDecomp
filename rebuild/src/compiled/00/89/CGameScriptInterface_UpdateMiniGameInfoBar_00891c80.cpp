struct Inner {
    void method(int a, int b, int c);
};
struct Holder {
    unsigned char pad[0x1c8];
    Inner* p1c8;
};
extern Holder* g_holder;

void __stdcall CGameScriptInterface_UpdateMiniGameInfoBar(int param)
{
    g_holder->p1c8->method(param, 0, 0);
}