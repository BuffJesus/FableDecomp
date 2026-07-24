struct Inner { void Do(int a, int b, int count, int arg); };  // Do = 0xffe23570 (thiscall)
struct CMid { char pad[0x18]; Inner* p18; };
struct CGameScriptInterface { void* vt; CMid* mid; };

extern void* __stdcall Resolve(int a);          // 0xffbbc620
extern int __fastcall CountOf(void* self);      // 0x11d380

void __fastcall CGameScriptInterface_SetEnvironmentThemeWeightExternals(CGameScriptInterface* self, int edx, int arg1, int arg2)
{
    void* r = Resolve(arg1);
    int count = CountOf(r);
    if (count > 0) {
        self->mid->p18->Do(0, 1, count, arg2);
    }
}