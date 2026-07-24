#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;
class CASoundBank;

class CASoundBank
{
public:
    static long __cdecl GetSoundIndexFromSymbol(
        CASoundBank* pSoundBank,
        const CCharString* pSymbol);
};

class CGameScriptInterface
{
public:
    std::uint32_t __thiscall Play2DSound(const CCharString& soundSymbol) const;
};

extern void* DAT_013b86a0;
extern void* DAT_013b8394;
extern std::uint32_t DAT_0129d25c;

namespace
{
struct SoundGlobalsOverlay
{
    std::byte m_Pad00[0x10];
    CASoundBank* m_pSoundBank;
};

static_assert(offsetof(SoundGlobalsOverlay, m_pSoundBank) == 0x10);

struct Play2DSoundParams
{
    float m_Field00;
    float m_Field04;
    std::uint32_t m_Field08;
    std::uint32_t m_Field0C;
    std::uint32_t m_Field10;
    std::uint32_t m_Field14;
    std::uint32_t m_Field18;
    std::int32_t m_Field1C;
};

static_assert(offsetof(Play2DSoundParams, m_Field00) == 0x00);
static_assert(offsetof(Play2DSoundParams, m_Field04) == 0x04);
static_assert(offsetof(Play2DSoundParams, m_Field08) == 0x08);
static_assert(offsetof(Play2DSoundParams, m_Field0C) == 0x0C);
static_assert(offsetof(Play2DSoundParams, m_Field10) == 0x10);
static_assert(offsetof(Play2DSoundParams, m_Field14) == 0x14);
static_assert(offsetof(Play2DSoundParams, m_Field18) == 0x18);
static_assert(offsetof(Play2DSoundParams, m_Field1C) == 0x1C);

struct Play2DSoundProviderVftableOverlay
{
    std::byte m_Pad00[0x24];
    void* (__thiscall* m_RawSlot24)(void* pThis);
};

static_assert(offsetof(Play2DSoundProviderVftableOverlay, m_RawSlot24) == 0x24);

struct Play2DSoundPlayerVftableOverlay
{
    std::byte m_Pad00[0x0C];
    std::uint32_t(__thiscall* m_RawSlot0C)(
        void* pThis,
        CASoundBank* pSoundBank,
        long soundIndex,
        std::uint32_t zero,
        Play2DSoundParams* pParams,
        std::uint32_t one);
};

static_assert(offsetof(Play2DSoundPlayerVftableOverlay, m_RawSlot0C) == 0x0C);
}

std::uint32_t __thiscall CGameScriptInterface::Play2DSound(
    const CCharString& soundSymbol) const
{
    const auto* const pSoundGlobals =
        reinterpret_cast<const SoundGlobalsOverlay*>(DAT_013b86a0);
    const long soundIndex =
        CASoundBank::GetSoundIndexFromSymbol(pSoundGlobals->m_pSoundBank, &soundSymbol);
    if (soundIndex == 0)
    {
        return DAT_0129d25c;
    }

    Play2DSoundParams params{
        1.0f,
        1.0f,
        0,
        1,
        1,
        0x8010,
        0,
        -1,
    };

    const auto* const pProviderVftable =
        *reinterpret_cast<const Play2DSoundProviderVftableOverlay* const*>(DAT_013b8394);
    void* const pPlayer = pProviderVftable->m_RawSlot24(DAT_013b8394);

    const auto* const pPlayerVftable =
        *reinterpret_cast<const Play2DSoundPlayerVftableOverlay* const*>(pPlayer);
    return pPlayerVftable->m_RawSlot0C(
        pPlayer,
        reinterpret_cast<const SoundGlobalsOverlay*>(DAT_013b86a0)->m_pSoundBank,
        soundIndex,
        0,
        &params,
        1);
}