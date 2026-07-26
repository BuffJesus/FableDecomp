#include <cstdio>

struct CSoundSys {
    int muted;
    void SetAllMuted();
};
static CSoundSys g_sys;
static int g_getarg = -999;
static int g_getcalls = 0;
static int g_setcalls = 0;

void CSoundSys::SetAllMuted() { g_setcalls++; this->muted = g_getarg ? 1 : 0; }
CSoundSys* __stdcall GetSoundSystem(int muted) { g_getcalls++; g_getarg = muted; return &g_sys; }

void __fastcall CGameScriptInterface_SetAllSoundsAsMuted(void* self, void* edx, int muted)
{
    GetSoundSystem(muted)->SetAllMuted();
}

int main()
{
    g_sys.muted = -1;
    CGameScriptInterface_SetAllSoundsAsMuted(0, 0, 1);
    if (g_sys.muted == 1 && g_getcalls == 1 && g_setcalls == 1 && g_getarg == 1) {
        CGameScriptInterface_SetAllSoundsAsMuted(0, 0, 0);
        if (g_sys.muted == 0 && g_getcalls == 2 && g_setcalls == 2 && g_getarg == 0) {
            std::printf("CGameScriptInterface_0088e090_TEST PASS\n");
            return 0;
        }
    }
    std::printf("FAIL\n");
    return 1;
}