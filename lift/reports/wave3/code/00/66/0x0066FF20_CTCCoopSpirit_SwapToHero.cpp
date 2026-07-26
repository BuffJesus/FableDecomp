#include <cstddef>
#include <cstdint>

namespace
{
struct CTCCoopSpiritField04Overlay
{
    std::byte Pad00[0x04];
    CVertexBufferWin32* Field04;
};

static_assert(offsetof(CTCCoopSpiritField04Overlay, Field04) == 0x04);

struct CoopSpiritThingOverlay
{
    std::byte Pad00[0x90];
    std::int8_t HeroSlot;
    std::byte Pad91[0x1F];
    std::int32_t FieldB0;
    std::int32_t FieldB4;
};

static_assert(offsetof(CoopSpiritThingOverlay, HeroSlot) == 0x90);
static_assert(offsetof(CoopSpiritThingOverlay, FieldB0) == 0xB0);
static_assert(offsetof(CoopSpiritThingOverlay, FieldB4) == 0xB4);

struct PositionWordsOverlay
{
    std::uint32_t Word00;
    std::uint32_t Word04;
    std::uint32_t Word08;
};

static_assert(sizeof(PositionWordsOverlay) == 0x0C);
static_assert(offsetof(PositionWordsOverlay, Word00) == 0x00);
static_assert(offsetof(PositionWordsOverlay, Word04) == 0x04);
static_assert(offsetof(PositionWordsOverlay, Word08) == 0x08);

struct Local10Overlay
{
    std::byte Bytes[0x10];
};

static_assert(sizeof(Local10Overlay) == 0x10);

struct ResolvedPlayerOverlay
{
    std::byte Pad00[0x60];
    void* Field60;
};

static_assert(offsetof(ResolvedPlayerOverlay, Field60) == 0x60);

using SwapToHeroRaw = void(__thiscall*)(CPlayerManager*, int);
using GetSpiritDefNameRaw = void*(__thiscall*)(CPlayerManager*, Local10Overlay*, CIDrawEnvironment*);
using AddChildRaw = void(__thiscall*)(CDisplayEngine*, CIDrawEnvironment*, void*);
using Local10PostCallRaw = void(__thiscall*)(Local10Overlay*);
using GetPlayerRaw = CPlayer*(__thiscall*)(CPlayerManager*, CIDrawEnvironment*);
using ResolvePlayerRaw = ResolvedPlayerOverlay*(__thiscall*)(CPlayer*);
using Field60VFuncRaw = void(__thiscall*)(void*, const PositionWordsOverlay*);

inline constexpr SwapToHeroRaw kSwapToHero =
    reinterpret_cast<SwapToHeroRaw>(0x00449D60);
inline constexpr GetSpiritDefNameRaw kGetSpiritDefName =
    reinterpret_cast<GetSpiritDefNameRaw>(0x00449730);
inline constexpr AddChildRaw kAddChild =
    reinterpret_cast<AddChildRaw>(0x00449B20);
inline constexpr Local10PostCallRaw kLocal10PostCall =
    reinterpret_cast<Local10PostCallRaw>(0x0099EB49);
inline constexpr GetPlayerRaw kGetPlayer =
    reinterpret_cast<GetPlayerRaw>(0x0044991A);
inline constexpr ResolvePlayerRaw kResolvePlayer =
    reinterpret_cast<ResolvePlayerRaw>(0x00487E13);
}

void __thiscall CTCCoopSpirit::SwapToHero()
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

    CDisplayEngine* const displayEngine =
        NDisplayView::CViewBase::GetDisplayEngine(
            reinterpret_cast<NDisplayView::CViewBase*>(this));
    CIDrawEnvironment* const drawEnvironment =
        CWorld::DrawGetEnvironment(reinterpret_cast<CWorld*>(displayEngine));

    auto* const spiritThing =
        reinterpret_cast<CoopSpiritThingOverlay*>(
            reinterpret_cast<CTCCoopSpiritField04Overlay*>(this)->Field04);
    auto* const positionSource =
        reinterpret_cast<PositionWordsOverlay*>(
            CVertexBufferWin32::DoSizeof(
                reinterpret_cast<CTCCoopSpiritField04Overlay*>(this)->Field04));

    PositionWordsOverlay localPosition;
    localPosition.Word00 = positionSource->Word00;
    localPosition.Word04 = positionSource->Word04;
    localPosition.Word08 = positionSource->Word08;

    const int heroSlot = static_cast<int>(spiritThing->HeroSlot);

    CDisplayEngine* const displayEngineForSwap =
        NDisplayView::CViewBase::GetDisplayEngine(
            reinterpret_cast<NDisplayView::CViewBase*>(this));
    kSwapToHero(
        reinterpret_cast<CPlayerManager*>(displayEngineForSwap),
        heroSlot);

    Local10Overlay local10;

    CDisplayEngine* const displayEngineForSpiritDef =
        NDisplayView::CViewBase::GetDisplayEngine(
            reinterpret_cast<NDisplayView::CViewBase*>(this));
    void* const addChildArgument =
        kGetSpiritDefName(
            reinterpret_cast<CPlayerManager*>(displayEngineForSpiritDef),
            &local10,
            drawEnvironment);

    CDisplayEngine* const displayEngineForAddChild =
        NDisplayView::CViewBase::GetDisplayEngine(
            reinterpret_cast<NDisplayView::CViewBase*>(this));
    kAddChild(displayEngineForAddChild, drawEnvironment, addChildArgument);

    kLocal10PostCall(&local10);

    CDisplayEngine* const displayEngineForGetPlayer =
        NDisplayView::CViewBase::GetDisplayEngine(
            reinterpret_cast<NDisplayView::CViewBase*>(this));
    CPlayer* const player =
        kGetPlayer(
            reinterpret_cast<CPlayerManager*>(displayEngineForGetPlayer),
            drawEnvironment);
    ResolvedPlayerOverlay* const resolvedPlayer = kResolvePlayer(player);

    void* const field60Object = resolvedPlayer->Field60;
    auto const field60Call =
        reinterpret_cast<Field60VFuncRaw>(
            (*reinterpret_cast<void***>(field60Object))[0x7C / sizeof(void*)]);
    field60Call(field60Object, &localPosition);

    spiritThing->FieldB4 = spiritThing->FieldB0;
}