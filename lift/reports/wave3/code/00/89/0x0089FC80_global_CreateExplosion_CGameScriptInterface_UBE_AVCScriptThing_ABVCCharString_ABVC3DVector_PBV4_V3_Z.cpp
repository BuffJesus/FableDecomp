#include <cstddef>
#include <cstdint>

class CCharString;
class CDefinitionManager;
class CGameScriptInterface;
class CScriptThing;
class CThing;

class C3DVector
{
public:
    float x;
    float y;
    float z;

    C3DVector& __thiscall operator=(const C3DVector& other);
    __thiscall ~C3DVector();
};

class CDefinitionManager
{
public:
    long __thiscall GetDefGlobalIndexFromName(const CCharString* pName);
};

class CTCDExplosion
{
public:
    static CThing* __fastcall Create(long definitionIndex, const C3DVector* pPosition);
    static CThing* __fastcall Create(
        long definitionIndex,
        const C3DVector* pPosition,
        const C3DVector* pVelocity);
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

extern "C" CDefinitionManager* __cdecl GFGetPreMainMemoryUsed();

namespace
{
using TExplosionPredicate =
    NScript::CPredicate_And<
        NScript::CPredicate_And<
            NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
            NScript::CIsThingAlive>,
        NScript::CIsThingAlive>;

struct CTCDExplosionThingOverlay
{
    std::byte m_Pad00[0x74];
    C3DVector m_Vector74;   // 0x74
    std::byte m_Pad80[0x11];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(CTCDExplosionThingOverlay, m_Vector74) == 0x74);
static_assert(offsetof(CTCDExplosionThingOverlay, m_Flags91) == 0x91);

struct CScriptThingOverlay
{
    void* m_Vfptr;          // 0x00
    std::uint32_t m_Field4; // 0x04
    std::uint32_t m_Field8; // 0x08
};
static_assert(offsetof(CScriptThingOverlay, m_Field4) == 0x04);
static_assert(offsetof(CScriptThingOverlay, m_Field8) == 0x08);
} // namespace

CScriptThing* __thiscall CGameScriptInterface::CreateExplosion(
    CScriptThing* pResult,
    const CCharString& definitionName,
    const C3DVector& position,
    const C3DVector* pVelocity,
    C3DVector vector74Value) const
{
    CDefinitionManager* const pDefinitionManager = GFGetPreMainMemoryUsed();
    const long definitionIndex = pDefinitionManager->GetDefGlobalIndexFromName(&definitionName);

    if (definitionIndex > 0)
    {
        CThing* pExplosion;
        if (pVelocity == nullptr)
        {
            pExplosion = CTCDExplosion::Create(definitionIndex, &position);
        }
        else
        {
            pExplosion = CTCDExplosion::Create(definitionIndex, &position, pVelocity);
        }

        if (pExplosion != nullptr)
        {
            auto* const pExplosionOverlay =
                reinterpret_cast<CTCDExplosionThingOverlay*>(pExplosion);

            if ((pExplosionOverlay->m_Flags91 & 0x01) == 0)
            {
                pExplosionOverlay->m_Vector74 = vector74Value;
                NScript::GFPredicateAnd<TExplosionPredicate>(pExplosion, pResult);
                return pResult;
            }
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