#include "engine/CGameScriptInterface.h"
#include <cstdio>

struct Inner {
    int applied; int one; int count; int arg2;
    void Apply(int a, int b, int c) { applied = 1; one = a; count = b; arg2 = c; }
};
struct CWorld { char pad[0x18]; Inner* p18; };

struct ThemeObj { int cnt; };

static ThemeObj g_theme;
static int g_lastChannel;

ThemeObj* __stdcall GetTheme(int channel) { g_lastChannel = channel; return &g_theme; }
int __fastcall ThemeCount(ThemeObj* self) { return self->cnt; }

void __fastcall CGameScriptInterface_SetEnvironmentThemeWeightAllChannels(
    CGameScriptInterface* self, int edx, int channel, int arg2)
{
    ThemeObj* t = GetTheme(channel);
    int n = ThemeCount(t);
    if (n > 0) {
        self->World->p18->Apply(1, n, arg2);
    }
}

int main() {
    Inner innerObj; innerObj.applied = 0;
    CWorld midObj; midObj.p18 = &innerObj;
    CGameScriptInterface obj; obj.World = &midObj;

    g_theme.cnt = 5;
    CGameScriptInterface_SetEnvironmentThemeWeightAllChannels(&obj, 0, 7, 99);
    if (g_lastChannel != 7) { std::printf("FAIL channel\n"); return 1; }
    if (innerObj.applied == 0 || innerObj.one != 1 || innerObj.count != 5 || innerObj.arg2 != 99) {
        std::printf("FAIL apply\n"); return 1;
    }

    innerObj.applied = 0;
    g_theme.cnt = 0;
    CGameScriptInterface_SetEnvironmentThemeWeightAllChannels(&obj, 0, 3, 42);
    if (innerObj.applied != 0) { std::printf("FAIL noapply\n"); return 1; }

    std::printf("CGameScriptInterface_00890010_TEST_PASS\n");
    return 0;
}