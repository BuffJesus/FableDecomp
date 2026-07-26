#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    float __thiscall GetGuildSealRecallAngleXY() const;
};

namespace
{
struct CGameScriptInterface_GetGuildSealRecallAngleXY_ThisOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(offsetof(CGameScriptInterface_GetGuildSealRecallAngleXY_ThisOverlay, m_Field04) == 0x04);

struct CGameScriptInterface_GetGuildSealRecallAngleXY_Field04Overlay
{
    std::byte m_Pad00[0x160];
    float m_Field160; // 0x160
};
static_assert(offsetof(CGameScriptInterface_GetGuildSealRecallAngleXY_Field04Overlay, m_Field160) == 0x160);
}

float __thiscall CGameScriptInterface::GetGuildSealRecallAngleXY() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetGuildSealRecallAngleXY_ThisOverlay*>(this);
    const auto* const field04 =
        reinterpret_cast<const CGameScriptInterface_GetGuildSealRecallAngleXY_Field04Overlay*>(self->m_Field04);
    return field04->m_Field160;
}