#include <cstddef>
#include <cstdint>

class C3DVector;
class CCharString;
class CGameScriptInterface;
class CASoundBank;

class CASoundBank
{
public:
    long __thiscall GetSoundIndexFromSymbol(const CCharString* pSymbol);
};

class CGameScriptInterface
{
public:
    std::uint32_t __thiscall PlaySoundAtPos(
        const C3DVector& position,
        const CCharString& soundSymbol) const;
};

extern void* DAT_013b86a0;
extern std::uint32_t DAT_0129d25c;

namespace
{
struct SoundGlobalsOverlay
{
    std::byte m_Pad00[0x10];
    CASoundBank* m_pSoundBank; // 0x10
};

static_assert(offsetof(SoundGlobalsOverlay, m_pSoundBank) == 0x10);

struct AttachedEmitterOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(AttachedEmitterOverlay, m_Flags91) == 0x91);

struct PlaySoundAtPosParams
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

static_assert(offsetof(PlaySoundAtPosParams, m_Field00) == 0x00);
static_assert(offsetof(PlaySoundAtPosParams, m_Field04) == 0x04);
static_assert(offsetof(PlaySoundAtPosParams, m_Field08) == 0x08);
static_assert(offsetof(PlaySoundAtPosParams, m_Field0C) == 0x0C);
static_assert(offsetof(PlaySoundAtPosParams, m_Field10) == 0x10);
static_assert(offsetof(PlaySoundAtPosParams, m_Field14) == 0x14);
static_assert(offsetof(PlaySoundAtPosParams, m_Field18) == 0x18);
static_assert(offsetof(PlaySoundAtPosParams, m_Field1C) == 0x1C);

using Sound_CreateAttachedEmitterFn =
    AttachedEmitterOverlay* (__fastcall*)(const C3DVector* pPosition, std::uint32_t unusedEdx, std::uint32_t one);
using CASoundBank_PlaySoundAtPos_Helper00836810Fn = std::uint32_t(__thiscall*)(
    CASoundBank* pThis,
    AttachedEmitterOverlay* pEmitter,
    long soundIndex,
    std::uint32_t zero0,
    PlaySoundAtPosParams* pParams,
    std::uint32_t zero1);

inline constexpr auto Sound_CreateAttachedEmitter =
    reinterpret_cast<Sound_CreateAttachedEmitterFn>(0x0083E790);
inline constexpr auto CASoundBank_PlaySoundAtPos_Helper00836810 =
    reinterpret_cast<CASoundBank_PlaySoundAtPos_Helper00836810Fn>(0x00836810);
}

std::uint32_t __thiscall CGameScriptInterface::PlaySoundAtPos(
    const C3DVector& position,
    const CCharString& soundSymbol) const
{
    const auto* const pSoundGlobals =
        reinterpret_cast<const SoundGlobalsOverlay*>(DAT_013b86a0);
    const long soundIndex =
        pSoundGlobals->m_pSoundBank->GetSoundIndexFromSymbol(&soundSymbol);

    if (soundIndex != 0)
    {
        AttachedEmitterOverlay* const pEmitter =
            Sound_CreateAttachedEmitter(&position, 0, 1);
        if ((pEmitter != nullptr) && ((pEmitter->m_Flags91 & 0x01u) == 0))
        {
            PlaySoundAtPosParams params{
                1.0f,
                1.0f,
                0,
                1,
                1,
                0x8010,
                0,
                -1,
            };

            return CASoundBank_PlaySoundAtPos_Helper00836810(
                pSoundGlobals->m_pSoundBank,
                pEmitter,
                soundIndex,
                0,
                &params,
                0);
        }
    }

    return DAT_0129d25c;
}