#include <cstddef>

class CGameScriptInterface
{
public:
    void __thiscall SetSummonerDeathExplosionAffectsHero(bool affectsHero) const;
};

namespace
{
struct CGameScriptInterface_SetSummonerDeathExplosionAffectsHero_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_SetSummonerDeathExplosionAffectsHero_SelfOverlay,
        m_Field04) == 0x04);

struct CGameScriptInterface_SetSummonerDeathExplosionAffectsHero_Field04Overlay
{
    std::byte m_Pad00[0xE1];
    bool m_FieldE1; // 0xE1
};
static_assert(
    offsetof(
        CGameScriptInterface_SetSummonerDeathExplosionAffectsHero_Field04Overlay,
        m_FieldE1) == 0xE1);
}

void __thiscall
CGameScriptInterface::SetSummonerDeathExplosionAffectsHero(
    bool affectsHero) const
{
    auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_SetSummonerDeathExplosionAffectsHero_SelfOverlay*>(
            this);
    auto* const field04 =
        reinterpret_cast<
            CGameScriptInterface_SetSummonerDeathExplosionAffectsHero_Field04Overlay*>(
            self->m_Field04);

    field04->m_FieldE1 = affectsHero;
}