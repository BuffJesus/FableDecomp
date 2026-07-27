class CScriptThing;
class CGameScriptInterface;
class CCharString;
class C3DVector;
class CThingAICreature;
class CTCVillage;
struct CTCBase;

enum ETCInterfaceType : int;

template <typename K, typename V, typename Compare>
class CVectorMap;

template <typename A, typename B>
class CKeyPairCompareLess;

namespace NScript
{
    class CIsThingAlive;

    template <typename A, typename B>
    class CPredicate_And;
}

namespace NHeroInformationScreens
{
    class CBase
    {
    public:
        CBase();
    };
}

using AlivePredicateChain = NScript::CPredicate_And<
    NScript::CPredicate_And<
        NScript::CPredicate_And<
            NScript::CIsThingAlive,
            NScript::CIsThingAlive>,
        NScript::CIsThingAlive>,
    NScript::CIsThingAlive>;

struct RespawnContextOverlay
{
    std::byte pad_00[0x24];
    std::uint8_t flags_24;
    std::byte pad_25[0x44 - 0x25];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> interfaces_44;
    int* end_48;
    std::byte pad_4C[0x91 - 0x4C];
    std::uint8_t flags_91;
};

static_assert(offsetof(RespawnContextOverlay, flags_24) == 0x24);
static_assert(offsetof(RespawnContextOverlay, interfaces_44) == 0x44);
static_assert(offsetof(RespawnContextOverlay, end_48) == 0x48);
static_assert(offsetof(RespawnContextOverlay, flags_91) == 0x91);

struct CScriptThingOverlay
{
    void* vfptr;
    void* field_4;
    void* field_8;
};

static_assert(offsetof(CScriptThingOverlay, field_4) == 0x4);
static_assert(offsetof(CScriptThingOverlay, field_8) == 0x8);

// ABI-exact form with the hidden sret buffer modeled explicitly.
CScriptThing* __thiscall CGameScriptInterface::TryToRespawnDefNamed(
    CScriptThing* result,
    const CCharString& defName,
    const C3DVector& position) const
{
    using GetContextFn = void* (__thiscall*)(const CGameScriptInterface*);
    using PredicateIntoScriptThingFn = CScriptThing* (__thiscall*)(CThingAICreature*, CScriptThing*);

    auto* const vftable = *reinterpret_cast<void* const* const*>(this);
    auto* const context = static_cast<RespawnContextOverlay*>(
        reinterpret_cast<GetContextFn>(vftable[0x2C / sizeof(void*)])(this));

    if ((context != nullptr) &&
        ((context->flags_91 & 0x01) == 0) &&
        ((context->flags_24 & 0x04) != 0))
    {
        int key_storage = 0x22;
        auto* it = reinterpret_cast<int*>(
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>::LowerBound(
                &context->interfaces_44,
                reinterpret_cast<ETCInterfaceType*>(&key_storage)));

        if ((it == context->end_48) || (0x22 < *it))
        {
            it = context->end_48;
        }

        CThingAICreature* const creature =
            CTCVillage::TryToRespawnDefNamed(reinterpret_cast<CTCVillage*>(it[1]), defName, position);

        if (creature != nullptr)
        {
            reinterpret_cast<PredicateIntoScriptThingFn>(
                &NScript::GFPredicateAnd<
                    AlivePredicateChain,
                    NScript::CIsThingAlive>)(creature, result);
            return result;
        }
    }

    NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(result));

    auto* const out = reinterpret_cast<CScriptThingOverlay*>(result);
    out->vfptr = reinterpret_cast<void*>(0x01238C8C);
    out->field_4 = nullptr;
    out->field_8 = nullptr;
    return result;
}