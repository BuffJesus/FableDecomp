#include <cstddef>
#include <cstdint>

class C3DVector;
class CCharString;
class CDefinitionManager;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CThing;

enum ETCInterfaceType : std::int32_t
{
    ETCInterfaceType_ExperienceOrb = 0x69,
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

class CTCDCameraPoint
{
public:
    static CThing* __fastcall Create(long definitionIndex, const C3DVector* pPosition);
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

CDefinitionManager* __cdecl GFGetPreMainMemoryUsed();
void __thiscall CExperienceOrb_SetScale(CTCBase* pThis, long scale);

struct CThingInterfaceEntry
{
    ETCInterfaceType m_Type;
    CTCBase* m_Interface;
};

using CThingInterfaceMap =
    CVectorMap<ETCInterfaceType, CThingInterfaceEntry, CKeyPairCompareLess<ETCInterfaceType, CThingInterfaceEntry>>;

static_assert(offsetof(CThingInterfaceMap, m_Begin) == 0x0);
static_assert(offsetof(CThingInterfaceMap, m_End) == 0x4);

struct CThingCreateExperienceOrbOverlay
{
    std::byte m_Pad00[0x2C];
    std::uint32_t m_Flags2C;
    std::byte m_Pad30[0x14];
    CThingInterfaceMap m_Interfaces;
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91;
};

static_assert(offsetof(CThingCreateExperienceOrbOverlay, m_Flags2C) == 0x2C);
static_assert(offsetof(CThingCreateExperienceOrbOverlay, m_Interfaces) == 0x44);
static_assert(offsetof(CThingCreateExperienceOrbOverlay, m_Flags91) == 0x91);

struct CScriptThingOverlay
{
    void* m_Vfptr;
    std::uint32_t m_Value4;
    std::uint32_t m_Value8;
};

static_assert(offsetof(CScriptThingOverlay, m_Value4) == 0x4);
static_assert(offsetof(CScriptThingOverlay, m_Value8) == 0x8);

struct CTCBaseOverlay
{
    std::byte m_Pad00[0x1C];
    std::uint8_t m_Byte1C;
};

static_assert(offsetof(CTCBaseOverlay, m_Byte1C) == 0x1C);

CScriptThing* __thiscall CGameScriptInterface::CreateExperienceOrb(
    CScriptThing* pResult,
    const C3DVector& position,
    long scale) const
{
    long definitionIndex;
    {
        CCharString thingName("THING_EXPERIENCE_ORB", -1);
        CDefinitionManager* const pDefinitionManager = GFGetPreMainMemoryUsed();
        definitionIndex = pDefinitionManager->GetDefGlobalIndexFromName(&thingName);
    }

    if (definitionIndex > 0)
    {
        CThing* const pThing = CTCDCameraPoint::Create(definitionIndex, &position);
        if (pThing != nullptr)
        {
            auto* const pThingOverlay = reinterpret_cast<CThingCreateExperienceOrbOverlay*>(pThing);
            if ((pThingOverlay->m_Flags91 & 0x01) == 0)
            {
                CTCBase* pScaleTarget = reinterpret_cast<CTCBase*>(pResult);

                if ((pThingOverlay->m_Flags2C & 0x200) != 0)
                {
                    const ETCInterfaceType interfaceType = ETCInterfaceType_ExperienceOrb;
                    CThingInterfaceEntry* pEntry = pThingOverlay->m_Interfaces.LowerBound(&interfaceType);
                    if (pEntry == pThingOverlay->m_Interfaces.m_End || interfaceType < pEntry->m_Type)
                    {
                        pEntry = pThingOverlay->m_Interfaces.m_End;
                    }
                    pScaleTarget = pEntry->m_Interface;
                }

                CExperienceOrb_SetScale(pScaleTarget, scale);
                reinterpret_cast<CTCBaseOverlay*>(pScaleTarget)->m_Byte1C = 0;

                NScript::GFPredicateAnd<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<
                            NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                            NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>(pThing, pResult);
                return pResult;
            }
        }
    }

    NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(pResult));
    auto* const pResultOverlay = reinterpret_cast<CScriptThingOverlay*>(pResult);
    pResultOverlay->m_Vfptr = reinterpret_cast<void*>(0x01238C8C);
    pResultOverlay->m_Value4 = 0;
    pResultOverlay->m_Value8 = 0;
    return pResult;
}