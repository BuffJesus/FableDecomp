#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

class C3DVector;
class CCharString;
class CCreatureActionBase;
class CDefinitionManager;
class CGameScriptInterface;
class CScriptThing;
class CThingAICreature;
class CThingAICreatureInit;
class CThingCreatureBase;
class CThingPhysicalSwitchDef;
class CTCLightningOrb;
class CCombatAction_Guildmaster_HealFollowersInto;

enum EHeroMorphType : std::int32_t;

class CParticleMorphs
{
public:
    class CEntry;
};

template <typename T>
class CDefPointer;

namespace NHeroInformationScreens
{
class CBase
{
public:
    CBase();
};
}

namespace NScript
{
class CIsThingAlive;

template <typename TLeft, typename TRight>
class CPredicate_And;

template <typename TPredicate, typename TResult>
TResult* GFPredicateAnd(TPredicate*, TResult*);
}

extern void* PTR__scalar_deleting_destructor__01238c8c;
void operator_delete(void*);

namespace
{
using HeroMorphPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;

struct CCharStringStorage
{
    std::uint32_t raw;
};
static_assert(sizeof(CCharStringStorage) == 0x04);

struct CScriptThingShared_Overlay
{
    std::int32_t refCount; // 0x00
    void* release;         // 0x04
};
static_assert(offsetof(CScriptThingShared_Overlay, refCount) == 0x00);
static_assert(offsetof(CScriptThingShared_Overlay, release) == 0x04);
static_assert(sizeof(CScriptThingShared_Overlay) == 0x08);

struct CScriptThing_Overlay
{
    void* vfptr;                          // 0x00
    std::int32_t field_04;               // 0x04
    CScriptThingShared_Overlay* shared;  // 0x08
};
static_assert(offsetof(CScriptThing_Overlay, vfptr) == 0x00);
static_assert(offsetof(CScriptThing_Overlay, field_04) == 0x04);
static_assert(offsetof(CScriptThing_Overlay, shared) == 0x08);
static_assert(sizeof(CScriptThing_Overlay) == 0x0C);

struct CGameScriptInterface_CreateCreature_Overlay
{
    void** vfptr;                           // 0x00
    std::byte pad_04[0x10 - 0x04];
    CDefinitionManager* definitionManager; // 0x10
    std::byte pad_14[0x4C - 0x14];
    std::int32_t callbackCount;            // 0x4C
};
static_assert(offsetof(CGameScriptInterface_CreateCreature_Overlay, definitionManager) == 0x10);
static_assert(offsetof(CGameScriptInterface_CreateCreature_Overlay, callbackCount) == 0x4C);

struct CThingPhysicalSwitchDef_CreateCreature_Overlay
{
    void** vfptr;                        // 0x00
    std::int32_t refCount;              // 0x04
    std::byte pad_08[0x11C - 0x08];
    std::int32_t field_11C;             // 0x11C
};
static_assert(offsetof(CThingPhysicalSwitchDef_CreateCreature_Overlay, refCount) == 0x04);
static_assert(offsetof(CThingPhysicalSwitchDef_CreateCreature_Overlay, field_11C) == 0x11C);

struct CThingAICreatureInit_CreateCreature_Overlay
{
    std::int32_t field_00;     // 0x00
    std::int32_t field_04;     // 0x04
    CCharStringStorage field_08; // 0x08
    CCharStringStorage field_0C; // 0x0C
    std::uint8_t field_10;     // 0x10
    std::uint8_t field_11;     // 0x11
    std::uint8_t field_12;     // 0x12
    std::uint8_t field_13;     // 0x13
    std::int32_t field_14;     // 0x14
    std::int32_t field_18;     // 0x18
};
static_assert(offsetof(CThingAICreatureInit_CreateCreature_Overlay, field_08) == 0x08);
static_assert(offsetof(CThingAICreatureInit_CreateCreature_Overlay, field_0C) == 0x0C);
static_assert(offsetof(CThingAICreatureInit_CreateCreature_Overlay, field_10) == 0x10);
static_assert(offsetof(CThingAICreatureInit_CreateCreature_Overlay, field_14) == 0x14);
static_assert(offsetof(CThingAICreatureInit_CreateCreature_Overlay, field_18) == 0x18);
static_assert(sizeof(CThingAICreatureInit_CreateCreature_Overlay) == 0x1C);
}

CScriptThing* __thiscall CGameScriptInterface::CreateCreature(
    CScriptThing* result,
    const CCharString& definitionName,
    const C3DVector& position,
    const CCharString& creatureName,
    bool startGuildmasterHealAction) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_CreateCreature_Overlay*>(this);

    HeroMorphPair* extraout_EDX;
    HeroMorphPair* extraout_EDX_00;
    NScript::CIsThingAlive* extraout_EDX_01;
    HeroMorphPair* extraout_EDX_02;
    HeroMorphPair* extraout_EDX_03;
    HeroMorphPair* extraout_EDX_04;
    HeroMorphPair* ppVar7;
    HeroMorphPair* extraout_EDX_05;
    HeroMorphPair* unaff_EDI;

    const long defGlobalIndex =
        CDefinitionManager::GetDefGlobalIndexFromName(self->definitionManager, &definitionName);

    CScriptThing_Overlay local_14c;
    NHeroInformationScreens::CBase::CBase(
        reinterpret_cast<NHeroInformationScreens::CBase*>(&local_14c));
    local_14c.vfptr = &PTR__scalar_deleting_destructor__01238c8c;
    local_14c.field_04 = 0;
    local_14c.shared = nullptr;

    CThingPhysicalSwitchDef_CreateCreature_Overlay* local_150 = nullptr;
    const bool hasDef = CDefinitionManager::GetDef<CThingPhysicalSwitchDef>(
        self->definitionManager,
        defGlobalIndex,
        reinterpret_cast<CDefPointer<const CThingPhysicalSwitchDef>*>(&local_150));

    if (hasDef)
    {
        alignas(CCharString) CCharStringStorage local_140{};
        alignas(CCharString) CCharStringStorage local_150_string{};
        CThingAICreatureInit_CreateCreature_Overlay local_130{};

        CCharString::CCharString(reinterpret_cast<CCharString*>(&local_140), "", -1);
        CCharString::CCharString(reinterpret_cast<CCharString*>(&local_150_string), "", -1);

        local_130.field_00 = 0;
        local_130.field_04 = 0;
        CCharString::CCharString(
            reinterpret_cast<CCharString*>(&local_130.field_08),
            reinterpret_cast<CCharString*>(&local_150_string));
        CCharString::CCharString(
            reinterpret_cast<CCharString*>(&local_130.field_0C),
            reinterpret_cast<CCharString*>(&local_140));
        local_130.field_10 = 0;
        local_130.field_11 = 1;
        local_130.field_12 = 1;
        local_130.field_13 = 0;
        local_130.field_14 = 0;
        local_130.field_18 = 0;

        std::_Cons_val<
            std::allocator<HeroMorphPair>,
            HeroMorphPair,
            const HeroMorphPair&>(
            reinterpret_cast<std::allocator<HeroMorphPair>*>(&local_150_string),
            extraout_EDX,
            unaff_EDI);
        std::_Cons_val<
            std::allocator<HeroMorphPair>,
            HeroMorphPair,
            const HeroMorphPair&>(
            reinterpret_cast<std::allocator<HeroMorphPair>*>(&local_140),
            extraout_EDX_00,
            unaff_EDI);

        CCharString::operator=(
            reinterpret_cast<CCharString*>(&local_130.field_08),
            &creatureName);

        CThingAICreature* const creature = CThingAICreature::Create(
            defGlobalIndex,
            &position,
            local_150->field_11C,
            reinterpret_cast<CThingAICreatureInit*>(&local_130));

        CScriptThing_Overlay local_13c;
        CScriptThing* const predicateThing =
            NScript::GFPredicateAnd<
                NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>,
                NScript::CIsThingAlive>(
                reinterpret_cast<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<
                            NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                            NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>*>(creature),
                reinterpret_cast<CScriptThing*>(&local_13c));

        CScriptThing::operator=(
            reinterpret_cast<CScriptThing*>(&local_14c),
            predicateThing);

        local_13c.vfptr = &PTR__scalar_deleting_destructor__01238c8c;
        if (local_13c.shared != nullptr)
        {
            --local_13c.shared->refCount;
            if (local_13c.shared->refCount == 0)
            {
                reinterpret_cast<void(__thiscall*)(CScriptThingShared_Overlay*)>(
                    local_13c.shared->release)(local_13c.shared);
                operator_delete(local_13c.shared);
            }
        }
        local_13c.field_04 = 0;
        local_13c.shared = nullptr;
        NHeroInformationScreens::CBase::CBase(
            reinterpret_cast<NHeroInformationScreens::CBase*>(&local_13c));

        ppVar7 = extraout_EDX_02;
        if (startGuildmasterHealAction)
        {
            alignas(CCombatAction_Guildmaster_HealFollowersInto) std::byte actionStorage[276];

            CCreatureActionBase* const action =
                reinterpret_cast<CCreatureActionBase*>(
                    CCombatAction_Guildmaster_HealFollowersInto::
                        CCombatAction_Guildmaster_HealFollowersInto(
                            reinterpret_cast<CCombatAction_Guildmaster_HealFollowersInto*>(actionStorage),
                            reinterpret_cast<CThingCreatureBase*>(creature)));

            CThingCreatureBase::SetCurrentAction(
                reinterpret_cast<CThingCreatureBase*>(creature),
                action);

            CTCLightningOrb::~CTCLightningOrb(
                reinterpret_cast<CTCLightningOrb*>(actionStorage));

            ppVar7 = extraout_EDX_03;
        }

        std::int32_t remaining = self->callbackCount;
        if (remaining > 0)
        {
            do
            {
                reinterpret_cast<void(__thiscall*)(const CGameScriptInterface*)>(
                    self->vfptr[0x1C / sizeof(void*)])(this);
                --remaining;
                ppVar7 = extraout_EDX_04;
            } while (remaining != 0);
        }

        std::_Cons_val<
            std::allocator<HeroMorphPair>,
            HeroMorphPair,
            const HeroMorphPair&>(
            reinterpret_cast<std::allocator<HeroMorphPair>*>(&local_130.field_0C),
            ppVar7,
            unaff_EDI);
        std::_Cons_val<
            std::allocator<HeroMorphPair>,
            HeroMorphPair,
            const HeroMorphPair&>(
            reinterpret_cast<std::allocator<HeroMorphPair>*>(&local_130.field_08),
            extraout_EDX_05,
            unaff_EDI);
    }

    auto* const out = reinterpret_cast<CScriptThing_Overlay*>(result);
    out->vfptr = &PTR__scalar_deleting_destructor__01238c8c;
    out->field_04 = local_14c.field_04;
    out->shared = local_14c.shared;
    if (local_14c.shared != nullptr)
    {
        ++local_14c.shared->refCount;
    }

    if (local_150 != nullptr)
    {
        --local_150->refCount;
        if (local_150->refCount == 0)
        {
            reinterpret_cast<void(__thiscall*)(CThingPhysicalSwitchDef_CreateCreature_Overlay*)>(
                local_150->vfptr[1])(local_150);
        }
    }

    local_14c.vfptr = &PTR__scalar_deleting_destructor__01238c8c;
    if (local_14c.shared != nullptr)
    {
        --local_14c.shared->refCount;
        if (local_14c.shared->refCount == 0)
        {
            reinterpret_cast<void(__thiscall*)(CScriptThingShared_Overlay*)>(
                local_14c.shared->release)(local_14c.shared);
            operator_delete(local_14c.shared);
        }
    }
    local_14c.field_04 = 0;
    local_14c.shared = nullptr;
    NHeroInformationScreens::CBase::CBase(
        reinterpret_cast<NHeroInformationScreens::CBase*>(&local_14c));

    return result;
}