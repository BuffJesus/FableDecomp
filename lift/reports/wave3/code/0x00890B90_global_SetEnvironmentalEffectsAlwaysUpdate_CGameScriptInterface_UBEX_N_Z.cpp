#include <cstddef>

class CGameScriptInterface
{
public:
    void __thiscall SetEnvironmentalEffectsAlwaysUpdate(bool alwaysUpdate) const;
};

namespace
{
struct CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_SelfOverlay
{
    std::byte m_Pad00[0x08];
    void* m_Field08; // 0x08
};

static_assert(
    offsetof(
        CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_SelfOverlay,
        m_Field08) == 0x08);

struct CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_Field08Overlay
{
    std::byte m_Pad00[0x28];
    void* m_Field28; // 0x28
};

static_assert(
    offsetof(
        CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_Field08Overlay,
        m_Field28) == 0x28);

struct CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_Field28Overlay
{
    std::byte m_Pad00[0x2C];
    void* m_Field2C; // 0x2C
};

static_assert(
    offsetof(
        CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_Field28Overlay,
        m_Field2C) == 0x2C);

struct CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_IndirectTargetVTable
{
    std::byte m_Pad00[0x18];
    void (__thiscall* m_Slot18)(void*); // 0x18
};

static_assert(
    offsetof(
        CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_IndirectTargetVTable,
        m_Slot18) == 0x18);

struct CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_IndirectTargetOverlay
{
    CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_IndirectTargetVTable*
        m_VTable; // 0x00
};

static_assert(
    offsetof(
        CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_IndirectTargetOverlay,
        m_VTable) == 0x00);
}

void __thiscall CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate(
    bool /*alwaysUpdate*/) const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_SelfOverlay*>(
            this);
    const auto* const field08 =
        reinterpret_cast<
            const CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_Field08Overlay*>(
            self->m_Field08);
    const auto* const field28 =
        reinterpret_cast<
            const CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_Field28Overlay*>(
            field08->m_Field28);
    auto* const target =
        reinterpret_cast<
            CGameScriptInterface_SetEnvironmentalEffectsAlwaysUpdate_IndirectTargetOverlay*>(
            field28->m_Field2C);

    target->m_VTable->m_Slot18(target);
}