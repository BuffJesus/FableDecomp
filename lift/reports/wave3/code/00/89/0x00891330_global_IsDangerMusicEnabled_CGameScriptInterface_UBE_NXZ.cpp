#include <cstddef>

class CMusicManager
{
public:
    bool __thiscall IsDangerMusicEnabled() const;
};

class CGameScriptInterface
{
public:
    bool __thiscall IsDangerMusicEnabled() const;
};

namespace
{
struct CGameScriptInterface_IsDangerMusicEnabled_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};

static_assert(
    offsetof(
        CGameScriptInterface_IsDangerMusicEnabled_SelfOverlay,
        m_Field04) == 0x04);

struct CGameScriptInterface_IsDangerMusicEnabled_Field04Overlay
{
    std::byte m_Pad00[0x6C];
    CMusicManager* m_pMusicManager; // 0x6C
};

static_assert(
    offsetof(
        CGameScriptInterface_IsDangerMusicEnabled_Field04Overlay,
        m_pMusicManager) == 0x6C);
}

bool CGameScriptInterface::IsDangerMusicEnabled() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsDangerMusicEnabled_SelfOverlay*>(this);
    const auto* const field04 =
        reinterpret_cast<const CGameScriptInterface_IsDangerMusicEnabled_Field04Overlay*>(self->m_Field04);

    return field04->m_pMusicManager->IsDangerMusicEnabled();
}