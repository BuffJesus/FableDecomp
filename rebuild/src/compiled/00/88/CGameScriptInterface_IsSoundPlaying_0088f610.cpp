class CGameScriptInterface
{
public:
    bool __fastcall IsSoundPlaying(unsigned long soundId) const;
};

struct CGameScriptInterface_SoundProvider
{
    void** vtable;
};

extern CGameScriptInterface_SoundProvider* DAT_013B8394;

typedef bool (__fastcall *FnIsSoundPlaying)(CGameScriptInterface_SoundProvider*, unsigned long);

bool __fastcall CGameScriptInterface::IsSoundPlaying(unsigned long soundId) const
{
    return ((FnIsSoundPlaying)DAT_013B8394->vtable[15])(DAT_013B8394, soundId);
}