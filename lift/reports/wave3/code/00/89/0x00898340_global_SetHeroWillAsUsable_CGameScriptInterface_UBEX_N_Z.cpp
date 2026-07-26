#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CWorld;
class CIDrawEnvironment;
class CCategory;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(
        CVectorMap* pThis,
        const TKey* pKey);
};

class CWorld
{
public:
    CIDrawEnvironment* __thiscall DrawGetEnvironment();
};

namespace NInventory
{
class CTCInventoryBase
{
public:
    CCategory* __thiscall GetCategoryWithIndex(long index);
};
}

class CGameScriptInterface
{
public:
    void __thiscall SetHeroWillAsUsable(std::uint32_t param3Raw) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_SetHeroWillAsUsable_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_InterfaceContext; // 0x04
    std::byte m_Pad08[0x0C];
    void* m_Field14; // 0x14
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_SelfOverlay,
        m_InterfaceContext) == 0x04);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_SetHeroWillAsUsable_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_SetHeroWillAsUsable_TargetOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x1C];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_SetHeroWillAsUsable_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_TargetOverlay,
        m_Flags24) == 0x24);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_TargetOverlay,
        m_Flags91) == 0x91);

struct CGameScriptInterface_SetHeroWillAsUsable_HeroOverlay
{
    std::byte m_Pad00[0x0D];
    std::uint8_t m_Field0D; // 0x0D
    std::byte m_Pad0E[0x01];
    std::uint8_t m_Field0F; // 0x0F
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_HeroOverlay,
        m_Field0D) == 0x0D);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_HeroOverlay,
        m_Field0F) == 0x0F);

struct CGameScriptInterface_SetHeroWillAsUsable_CategoryOverlay
{
    std::byte m_Pad00[0x24];
    void* m_Field24; // 0x24
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_CategoryOverlay,
        m_Field24) == 0x24);

struct CGameScriptInterface_SetHeroWillAsUsable_CategoryStateOverlay
{
    void** m_pVftable;       // 0x00
    std::int32_t m_RefCount; // 0x04
    std::byte m_Pad08[0x69];
    std::uint8_t m_Field71; // 0x71
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_CategoryStateOverlay,
        m_RefCount) == 0x04);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroWillAsUsable_CategoryStateOverlay,
        m_Field71) == 0x71);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_SetHeroWillAsUsable_TargetOverlay* (__thiscall*)(void*);
using RawGetInterfaceFn = void* (__thiscall*)(void*);
using RawReleaseFn =
    void (__thiscall*)(CGameScriptInterface_SetHeroWillAsUsable_CategoryStateOverlay*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

void __thiscall CGameScriptInterface::SetHeroWillAsUsable(std::uint32_t param3Raw) const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_SetHeroWillAsUsable_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        auto* pHero =
            reinterpret_cast<CGameScriptInterface_SetHeroWillAsUsable_HeroOverlay*>(
                static_cast<std::uintptr_t>(param3Raw));

        if ((target->m_Flags24 & 0x00000200) != 0)
        {
            std::int32_t interfaceTypeValue = 0x29;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_SetHeroWillAsUsable_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(
                            target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x29 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            pHero =
                reinterpret_cast<CGameScriptInterface_SetHeroWillAsUsable_HeroOverlay*>(
                    pEntry->m_pInterface);
        }

        const bool isLowByteZero =
            static_cast<std::uint8_t>(param3Raw) == static_cast<std::uint8_t>(0);

        pHero->m_Field0F = static_cast<std::uint8_t>(isLowByteZero);

        if (isLowByteZero)
        {
            pHero->m_Field0D = 0;
        }
        else
        {
            auto* const interfaceObject = self->m_InterfaceContext;
            auto* const vftable = *reinterpret_cast<void***>(interfaceObject);
            const auto getInterface =
                reinterpret_cast<RawGetInterfaceFn>(vftable[0x30 / sizeof(void*)]);

            auto* const pWorld =
                reinterpret_cast<CWorld*>(getInterface(interfaceObject));
            CIDrawEnvironment* const pDrawEnvironment =
                pWorld->DrawGetEnvironment();

            auto* const pInventory =
                reinterpret_cast<NInventory::CTCInventoryBase*>(
                    getInterface(interfaceObject));
            auto* const pCategory =
                pInventory->GetCategoryWithIndex(
                    reinterpret_cast<long>(pDrawEnvironment));

            auto* const pCategoryState =
                reinterpret_cast<
                    CGameScriptInterface_SetHeroWillAsUsable_CategoryStateOverlay*>(
                    reinterpret_cast<
                        CGameScriptInterface_SetHeroWillAsUsable_CategoryOverlay*>(
                        pCategory)
                        ->m_Field24);

            if (pCategoryState != nullptr)
            {
                pCategoryState->m_RefCount = pCategoryState->m_RefCount + 1;

                if (static_cast<char>(pCategoryState->m_Field71) == '\0')
                {
                    pHero->m_Field0D = 1;
                }

                auto* const pRefCount = &pCategoryState->m_RefCount;
                *pRefCount = *pRefCount + -1;

                if (*pRefCount == 0)
                {
                    reinterpret_cast<RawReleaseFn>(pCategoryState->m_pVftable[1])(
                        pCategoryState);
                    return;
                }
            }
        }
    }
}