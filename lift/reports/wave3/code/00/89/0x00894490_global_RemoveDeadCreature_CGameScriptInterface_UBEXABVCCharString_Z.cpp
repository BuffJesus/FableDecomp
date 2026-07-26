#include <cstddef>
#include <new>

class CCharString
{
public:
    CCharString(const CCharString& other);
    ~CCharString();
};

extern "C" CCharString* __fastcall CCharString__AppendCString(
    void* destination,
    const CCharString* base,
    const char* suffix);

class CThing
{
public:
    void __thiscall Kill(bool immediately);
};

struct CThingFilter_HasDefGlobalIndex
{
    CCharString m_DefGlobalIndex; // 0x00
};

static_assert(offsetof(CThingFilter_HasDefGlobalIndex, m_DefGlobalIndex) == 0x00);
static_assert(sizeof(CThingFilter_HasDefGlobalIndex) == sizeof(CCharString));

class CThingSearchTools
{
public:
    template <typename TFilter, typename TThing>
    bool __thiscall GetPFirstThing(const TFilter* pFilter, TThing** ppThing);
};

class CGameScriptInterface
{
public:
    void __thiscall RemoveDeadCreature(const CCharString& creatureName) const;
};

namespace
{
struct CGameScriptInterface_RemoveDeadCreature_WorldOverlay
{
    std::byte m_Pad00[0x20];
    CThingSearchTools* m_pThingSearchTools; // 0x20
};

static_assert(
    offsetof(CGameScriptInterface_RemoveDeadCreature_WorldOverlay, m_pThingSearchTools) == 0x20);

struct CGameScriptInterface_RemoveDeadCreature_SelfOverlay
{
    std::byte m_Pad00[0x04];
    CGameScriptInterface_RemoveDeadCreature_WorldOverlay* m_pWorld; // 0x04
};

static_assert(
    offsetof(CGameScriptInterface_RemoveDeadCreature_SelfOverlay, m_pWorld) == 0x04);
}

void __thiscall CGameScriptInterface::RemoveDeadCreature(const CCharString& creatureName) const
{
    CThing* pThing = nullptr;
    alignas(CCharString) std::byte appendedStorage[sizeof(CCharString)];
    alignas(CThingFilter_HasDefGlobalIndex) std::byte filterStorage[sizeof(CThingFilter_HasDefGlobalIndex)];

    CCharString* const appended =
        CCharString__AppendCString(appendedStorage, &creatureName, "_DEAD_CREATURE");

    auto* const filter = reinterpret_cast<CThingFilter_HasDefGlobalIndex*>(filterStorage);
    new (&filter->m_DefGlobalIndex) CCharString(*appended);

    const bool found =
        reinterpret_cast<const CGameScriptInterface_RemoveDeadCreature_SelfOverlay*>(this)
            ->m_pWorld
            ->m_pThingSearchTools
            ->GetPFirstThing<CThingFilter_HasDefGlobalIndex, CThing>(filter, &pThing);

    filter->m_DefGlobalIndex.~CCharString();
    appended->~CCharString();

    if (found)
    {
        pThing->Kill(false);
    }
}