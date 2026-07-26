#include <cstddef>
#include <cstdint>

class C3DVector;
class CGameScriptInterface;
class CScriptThing;
class CThing;

class CCharString
{
public:
    CCharString& __thiscall operator=(const CCharString& other);
    __thiscall ~CCharString();
};

class CTCDPhysicalObstruction
{
public:
    static CThing* __cdecl Create(
        const C3DVector* pStart,
        const C3DVector* pEnd,
        float radius);
};

namespace NHeroInformationScreens
{
class CBase
{
public:
    __thiscall CBase();
};
}

namespace NScript
{
class CIsThingAlive;

template <typename TLeft, typename TRight>
class CPredicate_And;

template <typename TPredicate>
void __thiscall GFPredicateAnd(CThing* pThing, CScriptThing* pResult);
}

namespace
{
using TPhysicalBarrierPredicate =
    NScript::CPredicate_And<
        NScript::CPredicate_And<
            NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
            NScript::CIsThingAlive>,
        NScript::CIsThingAlive>;

struct CThingCreatePhysicalBarrierOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(CThingCreatePhysicalBarrierOverlay, m_Flags91) == 0x91);

struct CScriptThingOverlay
{
    void* m_Vfptr;          // 0x00
    std::uint32_t m_Field4; // 0x04
    std::uint32_t m_Field8; // 0x08
};
static_assert(offsetof(CScriptThingOverlay, m_Field4) == 0x04);
static_assert(offsetof(CScriptThingOverlay, m_Field8) == 0x08);
} // namespace

CScriptThing* __thiscall CGameScriptInterface::CreatePhysicalBarrier(
    CScriptThing* pResult,
    float radius,
    const C3DVector& start,
    const C3DVector* pEnd,
    CCharString barrierName) const
{
    if (pEnd == nullptr)
    {
        pEnd = &start;
    }

    CThing* const pThing = CTCDPhysicalObstruction::Create(&start, pEnd, radius);
    if (pThing != nullptr)
    {
        auto* const pThingOverlay = reinterpret_cast<CThingCreatePhysicalBarrierOverlay*>(pThing);
        if ((pThingOverlay->m_Flags91 & 0x01) == 0)
        {
            (*reinterpret_cast<CCharString*>(reinterpret_cast<std::byte*>(pThing) + 0x74)) = barrierName;
            NScript::GFPredicateAnd<TPhysicalBarrierPredicate>(pThing, pResult);
            return pResult;
        }
    }

    NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(pResult));

    auto* const pResultOverlay = reinterpret_cast<CScriptThingOverlay*>(pResult);
    pResultOverlay->m_Vfptr = reinterpret_cast<void*>(0x01238C8C);
    pResultOverlay->m_Field4 = 0;
    pResultOverlay->m_Field8 = 0;
    return pResult;
}