class CMusicManager
{
public:
    bool __fastcall IsDangerMusicEnabled() const;
};

struct CGSI_Inner
{
    char m_Pad00[0x6C];
    CMusicManager* m_pMusicManager;
};

struct CGSI_Self
{
    char m_Pad00[0x04];
    CGSI_Inner* m_Field04;
};

class CGameScriptInterface
{
public:
    virtual bool IsDangerMusicEnabled() const;
};

bool CGameScriptInterface::IsDangerMusicEnabled() const
{
    const CGSI_Self* self = reinterpret_cast<const CGSI_Self*>(this);
    return self->m_Field04->m_pMusicManager->IsDangerMusicEnabled();
}