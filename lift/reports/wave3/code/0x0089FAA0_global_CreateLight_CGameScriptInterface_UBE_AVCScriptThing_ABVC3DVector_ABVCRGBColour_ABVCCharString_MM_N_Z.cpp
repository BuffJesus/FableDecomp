#include <cstddef>
#include <cstdint>

class C3DVector;
class CCharString;
class CDefinitionManager;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CThing;
class CThingMarker;
class CRGBColour;

enum ETCInterfaceType : std::int32_t
{
    ETCInterfaceType_Light = 0x27,
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    TValue* m_Begin;
    TValue* m_End;

    TValue* __thiscall LowerBound(const TKey* pKey);
};

class CCharString
{
public:
    CCharString(const char* pText, int length);
    ~CCharString();
};

class CDefinitionManager
{
public:
    long __thiscall GetDefGlobalIndexFromName(const CCharString* pName);
};

class CThingMarker
{
public:
    static CThingMarker* __fastcall Create(
        long definitionIndex,
        const C3DVector* pPosition,
        const CCharString* pName);
};

class CTCLight
{
public:
    void __thiscall SetOverridden(std::uint32_t isOverriddenRaw);
    void __thiscall SetColour(const CRGBColour* pColour);
    void __thiscall SetFlicker(float value);
    void __thiscall SetActive(std::uint32_t isActiveRaw);
};

namespace NHeroInformationScreens
{
class CBase
{
public:
    CBase();
};
} // namespace NHeroInformationScreens

namespace NScript
{
class CIsThingAlive;

template <typename TLeft, typename TRight>
class CPredicate_And;

template <typename TPredicate, typename TResult>
void __thiscall GFPredicateAnd(CThing* pThing, TResult* pResult);
} // namespace NScript

namespace
{
struct CThingInterfaceEntry
{
    ETCInterfaceType m_Type;
    CTCBase* m_Interface;
};

using CThingInterfaceMap =
    CVectorMap<
        ETCInterfaceType,
        CThingInterfaceEntry,
        CKeyPairCompareLess<ETCInterfaceType, CThingInterfaceEntry>>;

struct CGameScriptInterface_CreateLight_SelfOverlay
{
    std::byte m_Pad00[0x10];
    CDefinitionManager* m_pDefinitionManager; // 0x10
};

static_assert(
    offsetof(
        CGameScriptInterface_CreateLight_SelfOverlay,
        m_pDefinitionManager) == 0x10);

struct CThingMarker_CreateLight_Overlay
{
    std::byte m_Pad00[0x24];
    std::int8_t m_Field24; // 0x24
    std::byte m_Pad25[0x1F];
    CThingInterfaceMap m_InterfaceMap; // 0x44
};

static_assert(
    offsetof(
        CThingMarker_CreateLight_Overlay,
        m_Field24) == 0x24);
static_assert(
    offsetof(
        CThingMarker_CreateLight_Overlay,
        m_InterfaceMap) == 0x44);

struct CScriptThingOverlay
{
    void* m_Vfptr;          // 0x00
    std::uint32_t m_Value4; // 0x04
    std::uint32_t m_Value8; // 0x08
};

static_assert(offsetof(CScriptThingOverlay, m_Value4) == 0x04);
static_assert(offsetof(CScriptThingOverlay, m_Value8) == 0x08);

using TCreateLightPredicate =
    NScript::CPredicate_And<
        NScript::CPredicate_And<
            NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
            NScript::CIsThingAlive>,
        NScript::CIsThingAlive>;
} // namespace

CScriptThing* __thiscall CGameScriptInterface::CreateLight(
    CScriptThing* pResult,
    const C3DVector& position,
    const CRGBColour& colour,
    const CCharString& name,
    float value6,
    float value7,
    bool value8) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_CreateLight_SelfOverlay*>(this);

    long definitionIndex;
    {
        CCharString markerName("MARKER_LIGHT", -1);
        definitionIndex = self->m_pDefinitionManager->GetDefGlobalIndexFromName(&markerName);
    }

    if (definitionIndex != 0)
    {
        CThingMarker* const pMarker = CThingMarker::Create(definitionIndex, &position, &name);
        if (pMarker != nullptr)
        {
            auto* const pMarkerOverlay =
                reinterpret_cast<CThingMarker_CreateLight_Overlay*>(pMarker);

            if (pMarkerOverlay->m_Field24 < 0)
            {
                const ETCInterfaceType interfaceType = ETCInterfaceType_Light;
                CThingInterfaceEntry* pEntry =
                    pMarkerOverlay->m_InterfaceMap.LowerBound(&interfaceType);

                if ((pEntry == pMarkerOverlay->m_InterfaceMap.m_End) ||
                    (interfaceType < pEntry->m_Type))
                {
                    pEntry = pMarkerOverlay->m_InterfaceMap.m_End;
                }

                CTCLight* const pLight = reinterpret_cast<CTCLight*>(pEntry->m_Interface);
                if (pLight != nullptr)
                {
                    pLight->SetOverridden(1);
                    pLight->SetColour(&colour);
                    pLight->SetFlicker(value6);
                    pLight->SetFlicker(value7);

                    const std::uint32_t rawValue8 =
                        static_cast<std::uint8_t>(value8);
                    pLight->SetFlicker(static_cast<float>(rawValue8));
                    pLight->SetActive(1);
                }
            }

            NScript::GFPredicateAnd<TCreateLightPredicate, NScript::CIsThingAlive>(
                reinterpret_cast<CThing*>(pMarker),
                pResult);
            return pResult;
        }
    }

    NHeroInformationScreens::CBase::CBase(
        reinterpret_cast<NHeroInformationScreens::CBase*>(pResult));

    auto* const pResultOverlay = reinterpret_cast<CScriptThingOverlay*>(pResult);
    pResultOverlay->m_Vfptr = reinterpret_cast<void*>(0x01238C8C);
    pResultOverlay->m_Value4 = 0;
    pResultOverlay->m_Value8 = 0;
    return pResult;
}