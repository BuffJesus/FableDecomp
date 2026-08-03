#include <cstddef>
#include <cstdint>
#include <new>
#include <vector>

struct CGameScriptInterface_SearchRootOverlay
{
    std::byte pad_00[0x20];
    CThingSearchTools* pThingSearchTools;
};
static_assert(offsetof(CGameScriptInterface_SearchRootOverlay, pThingSearchTools) == 0x20);

struct CGameScriptInterface_Overlay
{
    std::byte pad_00[0x04];
    CGameScriptInterface_SearchRootOverlay* pSearchRoot;
};
static_assert(offsetof(CGameScriptInterface_Overlay, pSearchRoot) == 0x04);

struct CCharString_Overlay
{
    CBasicString<char>* pBasicString;
};
static_assert(offsetof(CCharString_Overlay, pBasicString) == 0x00);

struct CScriptThing_Overlay
{
    void* pScalarDeletingDestructor;
    int value_04;
    int* pShared;
};
static_assert(offsetof(CScriptThing_Overlay, pScalarDeletingDestructor) == 0x00);
static_assert(offsetof(CScriptThing_Overlay, value_04) == 0x04);
static_assert(offsetof(CScriptThing_Overlay, pShared) == 0x08);
static_assert(sizeof(CScriptThing_Overlay) == 0x0C);

struct StdVector_CScriptThing_Overlay
{
    CScriptThing_Overlay* pBegin;
    CScriptThing_Overlay* pEnd;
    CScriptThing_Overlay* pCapacityEnd;
};
static_assert(offsetof(StdVector_CScriptThing_Overlay, pBegin) == 0x00);
static_assert(offsetof(StdVector_CScriptThing_Overlay, pEnd) == 0x04);
static_assert(offsetof(StdVector_CScriptThing_Overlay, pCapacityEnd) == 0x08);
static_assert(sizeof(StdVector_CScriptThing_Overlay) == 0x0C);

extern void* PTR__scalar_deleting_destructor__01238c8c;

extern void std__vector___Reserve(std::vector<CScriptThing>& vec, int count);
extern void std__vector_InsertRange(
    void* insertAt,
    int constructedThing,
    void* stackSpill,
    int count,
    int one);

int CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(
    const CCharString& scriptName,
    const C3DVector& position,
    float radius,
    std::vector<CScriptThing>& outThings) const
{
    const int x = __ftol2(position.x);
    const int y = __ftol2(position.y);
    int r = __ftol2(radius);

    C2DBoxI roughArea{};
    roughArea.left = x - r;
    roughArea.top = y - r;
    roughArea.right = x + r;
    roughArea.bottom = y + r;

    std::vector<CThingCreatureBase*> roughMatches;
    CScriptThing_Overlay tempThing{};
    tempThing.pScalarDeletingDestructor = nullptr;
    tempThing.value_04 = 0;
    tempThing.pShared = nullptr;

    alignas(CCharString) std::byte stackSpill[sizeof(CCharString)]{};

    const auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    CThingSearchTools* const pThingSearchTools = self->pSearchRoot->pThingSearchTools;

    CBasicString<char>* const pBasicString =
        reinterpret_cast<const CCharString_Overlay*>(&scriptName)->pBasicString;

    if (pBasicString == nullptr)
    {
        int compareCount = 1;
        bool equal = true;
        const char* lhs = "";
        const char* rhs = "";

        do
        {
            if (compareCount == 0)
            {
                break;
            }

            --compareCount;
            equal = (*lhs == *rhs);
            ++lhs;
            ++rhs;
        } while (equal);

        if (equal)
        {
            pThingSearchTools->GetPBestThingInRoughArea<
                CThingFilter_IsPotentialCustomer,
                CThingCompare_Nearest,
                CThingCreatureBase>(
                    roughArea,
                    reinterpret_cast<const CThingFilter_IsPotentialCustomer*>(&position),
                    reinterpret_cast<CThingCompare_Nearest*>(&roughMatches));
            goto build_results;
        }
    }
    else
    {
        if (pBasicString->operator==(""))
        {
            pThingSearchTools->GetPBestThingInRoughArea<
                CThingFilter_IsPotentialCustomer,
                CThingCompare_Nearest,
                CThingCreatureBase>(
                    roughArea,
                    reinterpret_cast<const CThingFilter_IsPotentialCustomer*>(&position),
                    reinterpret_cast<CThingCompare_Nearest*>(&roughMatches));
            goto build_results;
        }
    }

    {
        CCharString* const scriptNameCopy = new (stackSpill) CCharString(scriptName);

        pThingSearchTools->GetPBestThingInRoughArea<
            CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>,
            CThingCompare_Nearest,
            CThingCreatureBase>(
                roughArea,
                reinterpret_cast<const CFilter_Bind2<CThingFilter_IsNot, CIsVeryCloseMotionlessCreature>*>(scriptNameCopy),
                reinterpret_cast<CThingCompare_Nearest*>(&roughMatches));

        scriptNameCopy->~CCharString();
    }

build_results:
    {
        CThingCreatureBase** const pMatchBegin = roughMatches.data();
        CThingCreatureBase** const pMatchEnd = pMatchBegin + roughMatches.size();
        auto* const outOverlay = reinterpret_cast<StdVector_CScriptThing_Overlay*>(&outThings);

        if (pMatchBegin == pMatchEnd)
        {
            return 0;
        }

        std__vector___Reserve(outThings, static_cast<int>(pMatchEnd - pMatchBegin));

        CThingCreatureBase** pCurrent = pMatchBegin;
        do
        {
            const int constructedThing = NScript::GFPredicateAnd<
                NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<
                            NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                            NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>>(
                        *pCurrent,
                        reinterpret_cast<NScript::CIsThingAlive*>(&tempThing));

            CScriptThing_Overlay* const pEnd = outOverlay->pEnd;
            if (pEnd == outOverlay->pCapacityEnd)
            {
                std__vector_InsertRange(
                    pEnd,
                    constructedThing,
                    stackSpill,
                    1,
                    1);
            }
            else
            {
                if (pEnd != nullptr)
                {
                    const auto* const pConstructedThing =
                        reinterpret_cast<const CScriptThing_Overlay*>(constructedThing);

                    pEnd->pScalarDeletingDestructor = PTR__scalar_deleting_destructor__01238c8c;
                    pEnd->value_04 = pConstructedThing->value_04;
                    pEnd->pShared = pConstructedThing->pShared;

                    if (pConstructedThing->pShared != nullptr)
                    {
                        ++*pConstructedThing->pShared;
                    }
                }

                outOverlay->pEnd = reinterpret_cast<CScriptThing_Overlay*>(
                    reinterpret_cast<std::byte*>(outOverlay->pEnd) + sizeof(CScriptThing_Overlay));
            }

            tempThing.pScalarDeletingDestructor = PTR__scalar_deleting_destructor__01238c8c;
            if (tempThing.pShared != nullptr)
            {
                --*tempThing.pShared;
                if (*tempThing.pShared == 0)
                {
                    const auto destroy =
                        reinterpret_cast<void(__thiscall*)(void*)>(
                            reinterpret_cast<void**>(tempThing.pShared)[1]);
                    destroy(tempThing.pShared);
                    operator delete(tempThing.pShared);
                }
            }

            tempThing.value_04 = 0;
            tempThing.pShared = nullptr;
            NHeroInformationScreens::CBase::CBase(
                reinterpret_cast<NHeroInformationScreens::CBase*>(&tempThing));

            ++pCurrent;
        } while (pCurrent != pMatchEnd);
    }

    return static_cast<int>(
        (reinterpret_cast<const std::byte*>(reinterpret_cast<const StdVector_CScriptThing_Overlay*>(&outThings)->pEnd) -
         reinterpret_cast<const std::byte*>(reinterpret_cast<const StdVector_CScriptThing_Overlay*>(&outThings)->pBegin)) /
        sizeof(CScriptThing_Overlay));
}