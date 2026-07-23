struct Inner;
extern void __fastcall Inner_Do(Inner* self, int e, int a, int b, int c);
struct Mid { char pad[0x18]; Inner* p18; };
struct Outer { char pad[4]; Mid* p4; };

void __fastcall CGameScriptInterface_ResetToDefaultThemeExternals(Outer* self, int edx, int arg)
{
    Inner* obj = self->p4->p18;
    Inner_Do(obj, edx, 0, 1, arg);
}