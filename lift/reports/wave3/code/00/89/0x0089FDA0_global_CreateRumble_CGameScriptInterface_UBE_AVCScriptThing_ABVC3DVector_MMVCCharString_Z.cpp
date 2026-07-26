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

class CTCDRumble
{
public:
    static CThing* __fastcall Create(
        const C3DVector* pPosition,
        std::uint32_t loweredMaskedParam2,
        float param5,
        CCharString rumbleName);
};

namespace NHeroInformationScreens
{
class CBase
{
public:
    __thiscall CBase();
};
} // namespace NHeroInformationScreens

namespace NScript
{
class CIsThingAlive;

template <typename TLeft, typename TRight>
class CPredicate_And;

template <typename TLeft, typename TRight>
void __thiscall GFPredicateAnd(
    CPredicate_And<TLeft, TRight>* pPredicate,
    CScriptThing* pResult);
} // namespace NScript

namespace
{
using TRumblePredicateLeft =
    NScript::CPredicate_And<
        NScript::CPredicate_And<
            NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
            NScript::CIsThingAlive>,
        NScript::CIsThingAlive>;

using TRumblePredicate =
    NScript::CPredicate_And<TRumblePredicateLeft, NScript::CIsThingAlive>;

struct CThingRumbleOverlay
{
    std::byte m_Pad00[0x74];
    CCharString m_Name;     // 0x74
    std::byte m_PadAfter[0x91 - 0x74 - sizeof(CCharString)];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThingRumbleOverlay, m_Name) == 0x74);
static_assert(offsetof(CThingRumbleOverlay, m_Flags91) == 0x91);

struct CScriptThingOverlay
{
    void* m_Vfptr;          // 0x00
    std::uint32_t m_Field4; // 0x04
    std::uint32_t m_Field8; // 0x08
};

static_assert(offsetof(CScriptThingOverlay, m_Field4) == 0x04);
static_assert(offsetof(CScriptThingOverlay, m_Field8) == 0x08);
} // namespace

CScriptThing* __fastcall CGameScriptInterface::CreateRumble(
    std::uint32_t loweredParam2,
    CScriptThing* pResult,
    const C3DVector* pPosition,
    float param5,
    CCharString rumbleName) const
{
    CThing* const pThing =
        CTCDRumble::Create(
            pPosition,
            loweredParam2 & 0xFFFFFF00u,
            param5,
            rumbleName);

    if (pThing != nullptr)
    {
        auto* const pThingOverlay = reinterpret_cast<CThingRumbleOverlay*>(pThing);
        if ((pThingOverlay->m_Flags91 & 0x01) == 0)
        {
            pThingOverlay->m_Name = rumbleName;

            NScript::GFPredicateAnd<TRumblePredicateLeft, NScript::CIsThingAlive>(
                reinterpret_cast<TRumblePredicate*>(pThing),
                pResult);
            return pResult;
        }
    }

    NHeroInformationScreens::CBase::CBase(
        reinterpret_cast<NHeroInformationScreens::CBase*>(pResult));

    auto* const pResultOverlay = reinterpret_cast<CScriptThingOverlay*>(pResult);
    pResultOverlay->m_Vfptr = reinterpret_cast<void*>(0x01238C8C);
    pResultOverlay->m_Field4 = 0;
    pResultOverlay->m_Field8 = 0;
    return pResult;
}