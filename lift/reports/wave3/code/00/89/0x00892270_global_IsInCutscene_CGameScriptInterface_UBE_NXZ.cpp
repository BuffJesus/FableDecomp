#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    std::uint32_t __thiscall IsInCutscene() const;
};

namespace
{
struct CGameScriptInterface_IsInCutscene_TargetOverlay
{
    std::byte m_Pad00[0xF6];
    std::uint8_t m_FieldF6; // 0xF6
};
static_assert(
    offsetof(CGameScriptInterface_IsInCutscene_TargetOverlay, m_FieldF6) == 0xF6);

extern CGameScriptInterface_IsInCutscene_TargetOverlay* DAT_013B8790;
}

std::uint32_t __thiscall CGameScriptInterface::IsInCutscene() const
{
    const std::uint32_t value = reinterpret_cast<std::uint32_t>(DAT_013B8790);
    return (value & 0xFFFFFF00u) | DAT_013B8790->m_FieldF6;
}