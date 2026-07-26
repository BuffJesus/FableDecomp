#include <cstddef>
#include <cstdint>

class CCodeSectionManager;
class CGameEvent;
class CGameScriptInterface;
class CTCDParticleEmitter;

enum EMusicSetType : std::int32_t;

template <typename T>
class CTBaseSingleton
{
public:
    static T* __cdecl Get();
};

class CGameEvent
{
public:
    static void __thiscall AddUBYTE(CGameEvent* pEvent, std::uint8_t value);
};

class CGameScriptInterface
{
public:
    void __thiscall OverrideMusic(EMusicSetType musicSetType, bool enabled, bool value) const;
};

extern CTCDParticleEmitter* DAT_013b8394;

namespace
{
struct CCodeSectionManager_Overlay
{
    std::byte m_Pad00[0x0C];
    std::uint8_t m_Field0C;   // 0x0C
    std::byte m_Pad0D[0x17];
    std::uint32_t m_Field24;  // 0x24
    std::byte m_Pad28[0xBC];
    std::uint32_t m_FieldE4;  // 0xE4
};

static_assert(offsetof(CCodeSectionManager_Overlay, m_Field0C) == 0x0C);
static_assert(offsetof(CCodeSectionManager_Overlay, m_Field24) == 0x24);
static_assert(offsetof(CCodeSectionManager_Overlay, m_FieldE4) == 0xE4);

struct CGameScriptInterface_Inner_Overlay
{
    std::byte m_Pad00[0x6C];
    void* m_Field6C; // 0x6C
};

static_assert(offsetof(CGameScriptInterface_Inner_Overlay, m_Field6C) == 0x6C);

struct CGameScriptInterface_Overlay
{
    std::byte m_Pad00[0x04];
    CGameScriptInterface_Inner_Overlay* m_pInner; // 0x04
};

static_assert(offsetof(CGameScriptInterface_Overlay, m_pInner) == 0x04);

struct CTCDParticleEmitter_Overlay
{
    void** m_pVftable; // 0x00
};

static_assert(offsetof(CTCDParticleEmitter_Overlay, m_pVftable) == 0x00);

using ParticleEmitterSlotB8Fn =
    void(__thiscall*)(CTCDParticleEmitter*, std::int32_t, std::uint32_t, float);

// Raw helper at 0x006BDEA0, matching the observed disabled-branch call shape:
// ecx = *( *(this + 4) + 0x6C ), pushes = param_2 then param_4.
using DisabledBranchHelperFn =
    void(__thiscall*)(void*, std::uint32_t, std::uint32_t);
}

void __thiscall CGameScriptInterface::OverrideMusic(
    const EMusicSetType musicSetType,
    const bool enabled,
    const bool value) const
{
    const auto* const pSelf =
        reinterpret_cast<const CGameScriptInterface_Overlay*>(this);

    if (enabled)
    {
        CTCDParticleEmitter* const pEmitter = DAT_013b8394;
        if (pEmitter != nullptr)
        {
            const auto* const pCodeSectionManager =
                reinterpret_cast<const CCodeSectionManager_Overlay*>(
                    CTBaseSingleton<CCodeSectionManager>::Get());

            std::uint32_t sectionValue;
            if (pCodeSectionManager->m_Field0C == 0)
            {
                sectionValue = pCodeSectionManager->m_Field24;
            }
            else
            {
                sectionValue = pCodeSectionManager->m_FieldE4;
            }

            const auto* const pEmitterOverlay =
                reinterpret_cast<const CTCDParticleEmitter_Overlay*>(pEmitter);
            const auto fn = reinterpret_cast<ParticleEmitterSlotB8Fn>(
                pEmitterOverlay->m_pVftable[0xB8 / sizeof(void*)]);
            fn(pEmitter, 0x100, sectionValue, 500.0f);
        }

        CGameEvent::AddUBYTE(
            reinterpret_cast<CGameEvent*>(pSelf->m_pInner->m_Field6C),
            static_cast<std::uint8_t>(musicSetType));
        return;
    }

    reinterpret_cast<DisabledBranchHelperFn>(0x006BDEA0)(
        pSelf->m_pInner->m_Field6C,
        static_cast<std::uint32_t>(musicSetType),
        static_cast<std::uint32_t>(value));
}