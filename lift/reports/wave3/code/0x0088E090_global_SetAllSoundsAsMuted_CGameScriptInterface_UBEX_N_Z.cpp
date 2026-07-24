class CCodeSectionManager;
class CUserProfileManager;

template <typename T>
class CTBaseSingleton
{
public:
    static T* __cdecl Get();
};

class CGameScriptInterface
{
public:
    virtual void __thiscall SetAllSoundsAsMuted(bool muted) const;
};

class CUserProfileManager
{
public:
    void __thiscall SetSoundAsMuted(bool muted);
};

void __thiscall CGameScriptInterface::SetAllSoundsAsMuted(bool muted) const
{
    CCodeSectionManager* const codeSectionManager =
        CTBaseSingleton<CCodeSectionManager>::Get();
    reinterpret_cast<CUserProfileManager*>(codeSectionManager)->SetSoundAsMuted(
        muted);
}