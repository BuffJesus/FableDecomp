struct Inner {
    void method(int a, int b, int c, int d);
};

struct Thing {
    int count();
};

struct CMid {
    unsigned char pad[0x18];
    Inner* p18; // +0x18
};

struct CGameScriptInterface {
    void* vt;   // +0x0
    CMid* mid;  // +0x4
};

extern "C" Thing* __stdcall GetTheme(int arg1);

void __fastcall CGameScriptInterface_TransitionToThemeExternals(CGameScriptInterface* self, int edx_unused, int arg1, int arg2)
{
    Thing* t = GetTheme(arg1);
    int n = t->count();
    if (n > 0) {
        self->mid->p18->method(0, 1, n, arg2);
    }
}