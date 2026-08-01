#include <stdio.h>

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

static bool g_flag = false;
bool __fastcall CMusicManager::IsDangerMusicEnabled() const
{
    return g_flag;
}

bool CGameScriptInterface::IsDangerMusicEnabled() const
{
    const CGSI_Self* self = reinterpret_cast<const CGSI_Self*>(this);
    return self->m_Field04->m_pMusicManager->IsDangerMusicEnabled();
}

int main()
{
    CMusicManager mm;
    CGSI_Inner inner;
    inner.m_pMusicManager = &mm;
    CGSI_Self self;
    self.m_Field04 = &inner;
    CGameScriptInterface* gsi = reinterpret_cast<CGameScriptInterface*>(&self);

    g_flag = false;
    bool r0 = gsi->CGameScriptInterface::IsDangerMusicEnabled();
    g_flag = true;
    bool r1 = gsi->CGameScriptInterface::IsDangerMusicEnabled();

    if (r0 == false && r1 == true)
        printf("DANGER_MUSIC_OK\n");
    else
        printf("DANGER_MUSIC_FAIL r0=%d r1=%d\n", (int)r0, (int)r1);
    return 0;
}