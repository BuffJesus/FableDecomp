#include <cstdio>

static int g_muteCalls = 0;
static int g_lastMute = -1;
static int g_stopCalled = 0;

struct CSoundMgr {
    void** vtbl;
    void SetMute(int muted) { g_muteCalls++; g_lastMute = muted; }
};

CSoundMgr* g_soundMgr = 0;
unsigned char g_soundsEnabledFlag = 0;

typedef void (__fastcall *StopFn)(CSoundMgr*);

void __fastcall StopImpl(CSoundMgr*) { g_stopCalled++; }

struct CGameScriptInterface { char pad; };

void __fastcall CGameScriptInterface_EnableSounds(CGameScriptInterface* self, void* edx, char enable)
{
    CSoundMgr* mgr = g_soundMgr;
    if (mgr == 0)
        return;
    if (enable) {
        mgr->SetMute(1);
        g_soundsEnabledFlag = 1;
        return;
    }
    (*(StopFn*)((char*)mgr->vtbl + 0x40))(mgr);
    g_soundMgr->SetMute(0);
    g_soundsEnabledFlag = 0;
}

int main() {
    CGameScriptInterface gsi;

    g_soundMgr = 0;
    CGameScriptInterface_EnableSounds(&gsi, 0, 1);
    if (g_muteCalls != 0 || g_soundsEnabledFlag != 0) { std::printf("FAIL null path\n"); return 1; }

    void* vt[32];
    for (int i = 0; i < 32; i++) vt[i] = 0;
    vt[0x40 / sizeof(void*)] = (void*)&StopImpl;
    CSoundMgr mgr;
    mgr.vtbl = vt;
    g_soundMgr = &mgr;

    g_muteCalls = 0; g_lastMute = -1; g_stopCalled = 0; g_soundsEnabledFlag = 0;
    CGameScriptInterface_EnableSounds(&gsi, 0, 1);
    if (g_muteCalls != 1 || g_lastMute != 1 || g_soundsEnabledFlag != 1 || g_stopCalled != 0) {
        std::printf("FAIL enable path\n"); return 1;
    }

    g_muteCalls = 0; g_lastMute = -1; g_stopCalled = 0; g_soundsEnabledFlag = 1;
    CGameScriptInterface_EnableSounds(&gsi, 0, 0);
    if (g_stopCalled != 1 || g_muteCalls != 1 || g_lastMute != 0 || g_soundsEnabledFlag != 0) {
        std::printf("FAIL disable path\n"); return 1;
    }

    std::printf("CGameScriptInterface_0088f620_TEST PASS\n");
    return 0;
}