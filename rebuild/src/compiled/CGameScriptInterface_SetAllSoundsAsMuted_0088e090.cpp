// void __fastcall CGameScriptInterface::SetAllSoundsAsMuted(bool muted)
struct CSoundSys {
    void SetAllMuted(); // __fastcall: this in ecx; reuses caller-pushed arg slot
};
// getter is __stdcall(int): caller pushes muted, getter returns object WITHOUT
// cleaning that slot (modeled so the same push feeds the following __fastcall call)
extern CSoundSys* __stdcall GetSoundSystem(int muted);

void __fastcall CGameScriptInterface_SetAllSoundsAsMuted(void* /*self*/, void* /*edx*/, int muted)
{
    GetSoundSystem(muted)->SetAllMuted();
}