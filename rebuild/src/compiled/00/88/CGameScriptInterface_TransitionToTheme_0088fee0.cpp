struct Inner {
    void Apply(int one, int n, int arg2);
};
struct CMid { unsigned char pad[0x18]; Inner* p18; };
struct CGameScriptInterface { void* vt; CMid* mid; };

extern void* __stdcall Resolve(int a);
extern int __fastcall CountOf(void* self);

void __fastcall CGameScriptInterface_TransitionToTheme(CGameScriptInterface* self, void* edx, int arg1, int arg2)
{
    void* r = Resolve(arg1);
    int n = CountOf(r);
    if (n > 0) {
        self->mid->p18->Apply(1, n, arg2);
    }
}