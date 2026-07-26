#include <cstddef>
#include <cstdint>

class CCodeSectionManager;
class CGameEvent;
class CGameScriptInterface;
class CTCDParticleEmitter;

template <typename T>
class CTBaseSingleton
{
public:
    static T* __cdecl Get();
};

class CGameScriptInterface
{
public:
    void __thiscall StopOverrideMusic(bool enabled) const;
};

extern CTCDParticleEmitter* DAT_013b8394;

namespace
{
struct CCodeSectionManager_StopOverrideMusic_Overlay
{
    std::byte m_Pad00[0x0C];
    std::uint8_t m_Field0C; // 0x0C
    std::byte m_Pad0D[0x1B];
    std::uint32_t m_Field28; // 0x28
    std::byte m_Pad2C[0xBC];
    std::uint32_t m_FieldE8; // 0xE8
};

static_assert(
    offsetof(CCodeSectionManager_StopOverrideMusic_Overlay, m_Field0C) == 0x0C);
static_assert(
    offsetof(CCodeSectionManager_StopOverrideMusic_Overlay, m_Field28) == 0x28);
static_assert(
    offsetof(CCodeSectionManager_StopOverrideMusic_Overlay, m_FieldE8) == 0xE8);

struct StopOverrideMusic_InnerOverlay
{
    std::byte m_Pad00[0x6C];
    CGameEvent* m_pEvent; // 0x6C
};

static_assert(offsetof(StopOverrideMusic_InnerOverlay, m_pEvent) == 0x6C);

struct CGameScriptInterface_StopOverrideMusic_SelfOverlay
{
    std::byte m_Pad00[0x04];
    StopOverrideMusic_InnerOverlay* m_pInner; // 0x04
};

static_assert(
    offsetof(CGameScriptInterface_StopOverrideMusic_SelfOverlay, m_pInner) == 0x04);

struct CTCDParticleEmitter_StopOverrideMusic_Overlay
{
    void** m_pVftable; // 0x00
};

static_assert(
    offsetof(CTCDParticleEmitter_StopOverrideMusic_Overlay, m_pVftable) == 0x00);

using StopOverrideMusic_SlotB8Fn =
    void(__thiscall*)(CTCDParticleEmitter*, std::int32_t, std::uint32_t, float);

// Observed call shape is only `ecx = pEvent; call ...` at both sites.
// Ghidra models an unresolved SI-sourced byte live-in as `unaff_SI`, but no
// normal second C++ argument is marshaled at either call site.
void __thiscall CGameEvent_AddUBYTE(CGameEvent* pEvent);
}

void __thiscall CGameScriptInterface::StopOverrideMusic(bool enabled) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_StopOverrideMusic_SelfOverlay*>(this);

    if (enabled)
    {
        CTCDParticleEmitter* const pEmitter = DAT_013b8394;
        if (pEmitter != nullptr)
        {
            const auto* const pCodeSectionManager =
                reinterpret_cast<const CCodeSectionManager_StopOverrideMusic_Overlay*>(
                    CTBaseSingleton<CCodeSectionManager>::Get());

            std::uint32_t sectionValue;
            if (pCodeSectionManager->m_Field0C == 0)
            {
                sectionValue = pCodeSectionManager->m_Field28;
            }
            else
            {
                sectionValue = pCodeSectionManager->m_FieldE8;
            }

            const auto* const pEmitterOverlay =
                reinterpret_cast<const CTCDParticleEmitter_StopOverrideMusic_Overlay*>(
                    pEmitter);
            const auto fn = reinterpret_cast<StopOverrideMusic_SlotB8Fn>(
                pEmitterOverlay->m_pVftable[0xB8 / sizeof(void*)]);
            fn(pEmitter, 0x100, sectionValue, 500.0f);
        }

        CGameEvent_AddUBYTE(self->m_pInner->m_pEvent);
        return;
    }

    CGameEvent_AddUBYTE(self->m_pInner->m_pEvent);
}