struct CSoundMgr {
    void** vtbl;
    void SetMute(int muted);
};

extern CSoundMgr* g_soundMgr;
extern unsigned char g_soundsEnabledFlag;

typedef void (__fastcall *StopFn)(CSoundMgr*);

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