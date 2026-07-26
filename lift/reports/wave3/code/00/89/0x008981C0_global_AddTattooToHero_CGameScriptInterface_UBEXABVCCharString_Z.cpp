#include <cstddef>
#include <cstdint>
#include <set>

class CCharString
{
public:
    CCharString(const char* text, int length);
    ~CCharString();
};
static_assert(sizeof(CCharString) == 0x04);

struct C3DVector
{
    float x;
    float y;
    float z;
};

class CDefinitionManager
{
public:
    long __thiscall GetDefGlobalIndexFromName(const CCharString* name);
};

class CGameScriptInterface;
class CRightHandedSet;
class CTCBase;
class CTCHeroMorph;
class CTCAppearanceMorphBase;
class CThing;
class CThingObject;
class CIEngine;
class CVertexBufferWin32;

extern "C" CDefinitionManager* __cdecl GFGetPreMainMemoryUsed();

enum ETCInterfaceType : std::int32_t
{
    ETCInterfaceType_HeroMorph = 0x03,
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(CVectorMap* pThis, const TKey* pKey);
};

class CThing
{
public:
    static void __thiscall Kill(CThing* pThis, bool destroyImmediately);
};

class CVertexBufferWin32
{
public:
    float* __thiscall DoSizeof() const;
};

class CThingObject
{
public:
    static CThingObject* __fastcall Create(
        long definitionIndex,
        C3DVector* position,
        int layer,
        const CRightHandedSet* transform,
        int zero,
        const CCharString* name);
};

class CTCHeroMorph
{
public:
    void __thiscall AddTattoo(CThing* tattooThing);
};

class CTCAppearanceMorphBase
{
public:
    void __thiscall UploadTextureMorphsToEngine(
        CIEngine* engine,
        std::set<long, std::less<long>, std::allocator<long>>* changedMorphs,
        bool forceFullUpload,
        float fadeAmount);
};

class CGameScriptInterface
{
public:
    void __thiscall AddTattooToHero(const CCharString& tattooName) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_AddTattooToHero_SelfOverlay
{
    std::byte pad_00[0x14];
    void* field_14;
};
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_SelfOverlay, field_14) == 0x14);

struct CGameScriptInterface_AddTattooToHero_TCInterfaceEntryOverlay
{
    std::int32_t key;
    CTCBase* value;
};
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_TCInterfaceEntryOverlay, key) == 0x00);
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_TCInterfaceEntryOverlay, value) == 0x04);

struct CGameScriptInterface_AddTattooToHero_TargetOverlay
{
    std::byte pad_00[0x20];
    std::uint8_t flags_20;
    std::byte pad_21[0x23];
    std::byte interface_map_storage[0x04];
    CGameScriptInterface_AddTattooToHero_TCInterfaceEntryOverlay* interface_map_end;
    std::byte pad_4C[0x14];
    void* field_60;
    std::byte pad_64[0x2C];
    std::int8_t field_90;
    std::uint8_t flags_91;
};
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_TargetOverlay, flags_20) == 0x20);
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_TargetOverlay, interface_map_storage) == 0x44);
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_TargetOverlay, interface_map_end) == 0x48);
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_TargetOverlay, field_60) == 0x60);
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_TargetOverlay, field_90) == 0x90);
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_TargetOverlay, flags_91) == 0x91);

struct CGameScriptInterface_AddTattooToHero_ThingOverlay
{
    std::byte pad_00[0x91];
    std::uint8_t flags_91;
};
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_ThingOverlay, flags_91) == 0x91);

struct CGameScriptInterface_AddTattooToHero_PositionSourceOverlay
{
    void** vftable;
};
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_PositionSourceOverlay, vftable) == 0x00);

struct CGameScriptInterface_AddTattooToHero_PositionScratch
{
    std::byte storage[4];
};

struct CGameScriptInterface_AddTattooToHero_GlobalOverlay
{
    std::byte pad_00[0x28];
    void* field_28;
};
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_GlobalOverlay, field_28) == 0x28);

struct CGameScriptInterface_AddTattooToHero_EngineSourceOverlay
{
    std::byte pad_00[0x2C];
    CIEngine* engine;
};
static_assert(offsetof(CGameScriptInterface_AddTattooToHero_EngineSourceOverlay, engine) == 0x2C);

using Fn_00449970 = void*(__thiscall*)(void*);
using Fn_00487DC0 = CGameScriptInterface_AddTattooToHero_TargetOverlay*(__thiscall*)(void*);
using RawPositionSlot118Fn =
    float*(__thiscall*)(CGameScriptInterface_AddTattooToHero_PositionSourceOverlay*, void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);

auto* const DAT_013B86A0 =
    reinterpret_cast<CGameScriptInterface_AddTattooToHero_GlobalOverlay**>(0x013B86A0);
}

void __thiscall CGameScriptInterface::AddTattooToHero(const CCharString& tattooName) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_AddTattooToHero_SelfOverlay*>(this);

    CDefinitionManager* const definitionManager = GFGetPreMainMemoryUsed();
    const long definitionIndex = definitionManager->GetDefGlobalIndexFromName(&tattooName);
    if (definitionIndex <= 0)
    {
        return;
    }

    auto* const target = CALL_00487DC0(CALL_00449970(self->field_14));
    if ((target == nullptr) || ((target->flags_91 & 0x01) != 0))
    {
        return;
    }

    auto* const positionSource =
        reinterpret_cast<CGameScriptInterface_AddTattooToHero_PositionSourceOverlay*>(target->field_60);
    const auto rawPositionSlot118 =
        reinterpret_cast<RawPositionSlot118Fn>(positionSource->vftable[0x118 / sizeof(void*)]);

    CGameScriptInterface_AddTattooToHero_PositionScratch positionScratch{};
    float* const center = rawPositionSlot118(positionSource, &positionScratch);
    const float centerX = center[0];
    const float centerY = center[1];
    const float centerZ = center[2];

    float* const size = reinterpret_cast<CVertexBufferWin32*>(target)->DoSizeof();

    C3DVector spawnPosition{};
    spawnPosition.z = centerZ + centerZ + size[2];
    spawnPosition.y = centerY + centerY + size[1];
    spawnPosition.x = centerX + centerX + size[0];

    CThingObject* tattooThing = nullptr;
    {
        CCharString emptyName("", -1);
        tattooThing = CThingObject::Create(
            definitionIndex,
            &spawnPosition,
            static_cast<int>(target->field_90),
            nullptr,
            0,
            &emptyName);
    }

    if (tattooThing != nullptr)
    {
        const auto* const tattooThingOverlay =
            reinterpret_cast<const CGameScriptInterface_AddTattooToHero_ThingOverlay*>(tattooThing);

        if ((tattooThingOverlay->flags_91 & 0x01) == 0)
        {
            if ((target->flags_20 & 0x08) != 0)
            {
                std::int32_t interfaceTypeValue = ETCInterfaceType_HeroMorph;

                auto* entry =
                    reinterpret_cast<CGameScriptInterface_AddTattooToHero_TCInterfaceEntryOverlay*>(
                        TCInterfaceMap::LowerBound(
                            reinterpret_cast<TCInterfaceMap*>(target->interface_map_storage),
                            reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

                if ((entry == target->interface_map_end) || (ETCInterfaceType_HeroMorph < entry->key))
                {
                    entry = target->interface_map_end;
                }

                reinterpret_cast<CTCHeroMorph*>(entry->value)->AddTattoo(
                    reinterpret_cast<CThing*>(tattooThing));

                if (((target->flags_91 & 0x01) == 0) && ((target->flags_20 & 0x08) != 0))
                {
                    interfaceTypeValue = ETCInterfaceType_HeroMorph;

                    entry =
                        reinterpret_cast<CGameScriptInterface_AddTattooToHero_TCInterfaceEntryOverlay*>(
                            TCInterfaceMap::LowerBound(
                                reinterpret_cast<TCInterfaceMap*>(target->interface_map_storage),
                                reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

                    if ((entry == target->interface_map_end) || (ETCInterfaceType_HeroMorph < entry->key))
                    {
                        entry = target->interface_map_end;
                    }

                    auto* const engineSource =
                        reinterpret_cast<CGameScriptInterface_AddTattooToHero_EngineSourceOverlay*>(
                            (*DAT_013B86A0)->field_28);

                    reinterpret_cast<CTCAppearanceMorphBase*>(entry->value)
                        ->UploadTextureMorphsToEngine(
                            engineSource->engine,
                            nullptr,
                            false,
                            0.0f);
                }
            }

            CThing::Kill(reinterpret_cast<CThing*>(tattooThing), false);
        }
    }
}