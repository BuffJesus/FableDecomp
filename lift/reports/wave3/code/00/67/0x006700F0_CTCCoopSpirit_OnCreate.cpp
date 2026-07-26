#include <cstddef>
#include <cstdint>
#include <utility>

namespace
{
struct CTCCoopSpiritField04Overlay
{
    std::byte Pad00[0x04];
    std::uint32_t Field04;
};

static_assert(offsetof(CTCCoopSpiritField04Overlay, Field04) == 0x04);

struct CField04DefIndexOverlay
{
    std::byte Pad00[0x8C];
    std::uint16_t DefIndex;
};

static_assert(offsetof(CField04DefIndexOverlay, DefIndex) == 0x8C);

struct Local8Overlay
{
    CDefClassBase* Slot0;
    CDefClassBase* Slot1;
};

static_assert(sizeof(Local8Overlay) == 0x08);
static_assert(offsetof(Local8Overlay, Slot0) == 0x00);
static_assert(offsetof(Local8Overlay, Slot1) == 0x04);

struct CRefCountedDefOverlay
{
    void** Vftable;
    std::int32_t RefCount;
};

static_assert(offsetof(CRefCountedDefOverlay, Vftable) == 0x00);
static_assert(offsetof(CRefCountedDefOverlay, RefCount) == 0x04);

using MorphEntryPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;
}

void __thiscall CTCCoopSpirit::OnCreate()
{
    static_assert(sizeof(CTCCoopSpirit) == 0x34);
    static_assert(offsetof(CTCCoopSpirit, NoMovementTimer) == 0x0C);
    static_assert(offsetof(CTCCoopSpirit, Score) == 0x10);
    static_assert(offsetof(CTCCoopSpirit, OffscreenCounter) == 0x14);
    static_assert(offsetof(CTCCoopSpirit, MovementVector) == 0x18);
    static_assert(offsetof(CTCCoopSpirit, Master) == 0x24);
    static_assert(offsetof(CTCCoopSpirit, PCoopSpiritDef) == 0x28);
    static_assert(offsetof(CTCCoopSpirit, PParticleEmitter) == 0x2C);
    static_assert(sizeof(void*) == 4);

    Local8Overlay local_8{};
    CBaseObject* pCVar3;
    CDefClassBase* pCVar2;

    CBaseIntelligentPointer::CBaseIntelligentPointer(
        reinterpret_cast<CBaseIntelligentPointer*>(&local_8));
    local_8.Slot0 = reinterpret_cast<CDefClassBase*>(&PTR__vector_deleting_destructor__01238c6c);
    CBaseIntelligentPointer::SetPItem(
        reinterpret_cast<CBaseIntelligentPointer*>(&local_8),
        static_cast<CBaseObject*>(nullptr));
    pCVar3 = CBaseIntelligentPointer::GetPItem(
        reinterpret_cast<CBaseIntelligentPointer*>(&local_8));
    CBaseIntelligentPointer::SetPItem(
        reinterpret_cast<CBaseIntelligentPointer*>(&this->PParticleEmitter),
        pCVar3);
    CBaseIntelligentPointer::~CBaseIntelligentPointer(
        reinterpret_cast<CBaseIntelligentPointer*>(&local_8));

    ::_E2();

    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::byte*>(this) + 0x20) = 0;
    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::byte*>(this) + 0x1C) = 0;
    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::byte*>(this) + 0x18) = 0;
    this->NoMovementTimer = 0;
    this->Master = nullptr;
    this->OffscreenCounter = 0;
    this->Score = 0;

    auto* const ppVar4 = reinterpret_cast<MorphEntryPair*>(
        static_cast<std::uintptr_t>(
            reinterpret_cast<const CField04DefIndexOverlay*>(
                static_cast<std::uintptr_t>(
                    reinterpret_cast<const CTCCoopSpiritField04Overlay*>(this)->Field04))
                ->DefIndex));
    auto* const pCVar5 =
        reinterpret_cast<CDefPointer<CSpecialAbilitiesDrunkennessDef_const_>*>(&local_8);

    local_8.Slot0 = nullptr;

    auto* const this_00 = std::_Cons_val<
        std::allocator<MorphEntryPair>,
        MorphEntryPair,
        MorphEntryPair const&>(
        reinterpret_cast<std::allocator<MorphEntryPair>*>(this),
        ppVar4,
        ppVar4);

    CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(
        this_00,
        static_cast<long>(reinterpret_cast<std::uintptr_t>(ppVar4)),
        pCVar5);

    pCVar2 = local_8.Slot0;
    CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef>(
        local_8.Slot0,
        reinterpret_cast<CDefPointer<CAICreatureWillPowerIndicatorDef_const_>*>(
            &this->PCoopSpiritDef));

    if (pCVar2 != nullptr)
    {
        auto* const pRefCountedDef = reinterpret_cast<CRefCountedDefOverlay*>(pCVar2);
        pRefCountedDef->RefCount = pRefCountedDef->RefCount + -1;
        if (pRefCountedDef->RefCount == 0)
        {
            reinterpret_cast<void(__thiscall*)(CDefClassBase*)>(
                pRefCountedDef->Vftable[1])(pCVar2);
        }
    }
}