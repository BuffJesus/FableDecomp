#include <cstddef>
#include <cstdint>
#include <new>

class CBaseObject;
class CBaseIntelligentPointer;
class CGameScriptInterface;
class CScriptThing;
class CThing;
class CVillageTaskCarryCrate;

class CBaseIntelligentPointer
{
public:
    static CBaseObject* __thiscall GetPItem(CBaseIntelligentPointer* pThis);
};

class CVillageTaskCarryCrate
{
public:
    void __thiscall AssignCreatureToTask(CThing* pThing);
};

namespace NHeroInformationScreens
{
class CBase
{
public:
    CBase();
};
}

class CGameScriptInterface
{
public:
    CScriptThing* __thiscall GetMostRecentValidUsedTarget(CScriptThing* pResult) const;
};

namespace
{
struct CBaseObjectOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CBaseObjectOverlay, m_Flags91) == 0x91);

struct CScriptThingEmptyOverlay
{
    void* m_pVftable;        // 0x00
    std::uint32_t m_Field04; // 0x04
    std::uint32_t m_Field08; // 0x08
};

static_assert(offsetof(CScriptThingEmptyOverlay, m_pVftable) == 0x00);
static_assert(offsetof(CScriptThingEmptyOverlay, m_Field04) == 0x04);
static_assert(offsetof(CScriptThingEmptyOverlay, m_Field08) == 0x08);
static_assert(sizeof(CScriptThingEmptyOverlay) == 0x0C);

struct MostRecentValidUsedTargetGlobalOverlay
{
    std::byte m_Pad00[0xA8];
    CBaseIntelligentPointer m_ItemA8; // 0xA8
};

static_assert(offsetof(MostRecentValidUsedTargetGlobalOverlay, m_ItemA8) == 0xA8);

using BuildScriptThingFromObjectFn = void(__thiscall*)(CBaseObject* pThis, CScriptThing* pResult);

inline constexpr auto BuildScriptThingFromObject =
    reinterpret_cast<BuildScriptThingFromObjectFn>(0x004C7E50);
inline constexpr auto kScalarDeletingDestructor_01238C8C =
    reinterpret_cast<void*>(0x01238C8C);

inline auto& g_MostRecentValidUsedTarget =
    *reinterpret_cast<CVillageTaskCarryCrate**>(0x013B89FC);
} // namespace

CScriptThing* __thiscall CGameScriptInterface::GetMostRecentValidUsedTarget(CScriptThing* pResult) const
{
    auto* const pItem = CBaseIntelligentPointer::GetPItem(
        &reinterpret_cast<MostRecentValidUsedTargetGlobalOverlay*>(g_MostRecentValidUsedTarget)
             ->m_ItemA8);

    if ((pItem != nullptr) &&
        ((reinterpret_cast<const CBaseObjectOverlay*>(pItem)->m_Flags91 & 0x01) == 0))
    {
        g_MostRecentValidUsedTarget->AssignCreatureToTask(nullptr);
        BuildScriptThingFromObject(pItem, pResult);
        return pResult;
    }

    ::new (static_cast<void*>(pResult)) NHeroInformationScreens::CBase();
    auto* const emptyResult = reinterpret_cast<CScriptThingEmptyOverlay*>(pResult);
    emptyResult->m_pVftable = kScalarDeletingDestructor_01238C8C;
    emptyResult->m_Field04 = 0;
    emptyResult->m_Field08 = 0;
    return pResult;
}