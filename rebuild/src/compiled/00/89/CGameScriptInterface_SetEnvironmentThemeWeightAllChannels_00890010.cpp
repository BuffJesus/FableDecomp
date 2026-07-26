struct Inner { void Apply(int one, int count, int arg2); };
struct CMid { char pad[0x18]; Inner* p18; };
struct CGameScriptInterface { void* vt; CMid* mid; };

struct ThemeObj;
extern ThemeObj* __stdcall GetTheme(int channel);
extern int __fastcall ThemeCount(ThemeObj* self);

void __fastcall CGameScriptInterface_SetEnvironmentThemeWeightAllChannels(
    CGameScriptInterface* self, int /*edx*/, int channel, int arg2)
{
    ThemeObj* t = GetTheme(channel);
    int n = ThemeCount(t);
    if (n > 0) {
        self->mid->p18->Apply(1, n, arg2);
    }
}