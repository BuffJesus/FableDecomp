#include <cstddef>
#include <cstdint>

class CScriptThing;

struct CCharStringRep
{
    const char* m_pString;   // 0x00
    std::int32_t m_Length;   // 0x04
};

static_assert(offsetof(CCharStringRep, m_pString) == 0x00);
static_assert(offsetof(CCharStringRep, m_Length) == 0x04);
static_assert(sizeof(CCharStringRep) == 0x08);

class CCharString
{
public:
    CCharStringRep* m_pRep; // 0x00

    CCharString(const char* pText, int length);
    ~CCharString();
};

static_assert(offsetof(CCharString, m_pRep) == 0x00);
static_assert(sizeof(CCharString) == 0x04);

class CGameScriptInterface
{
public:
    void __thiscall EntitySetStategroupEnabled(
        const CScriptThing& entity,
        const CCharString& stategroup_name,
        bool enabled) const;
};

namespace
{
struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    void* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CThingStateGroupOverlay
{
    std::byte m_Pad00[0x10];
    std::uint8_t m_Field10; // 0x10
};

static_assert(offsetof(CThingStateGroupOverlay, m_Field10) == 0x10);
}

void __thiscall CGameScriptInterface::EntitySetStategroupEnabled(
    const CScriptThing& entity,
    const CCharString& stategroup_name,
    bool enabled) const
{
    using EntitySetStategroupEnabledFn =
        void (__thiscall*)(void* pThing, const CCharString& stategroup_name, bool enabled);

    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    void* const pThing = entityOverlay->m_pVTable->GetThing(&entity);

    if (pThing != nullptr)
    {
        auto* const pThingOverlay = reinterpret_cast<CThingStateGroupOverlay*>(pThing);
        if (pThingOverlay->m_Field10 == 1)
        {
            reinterpret_cast<EntitySetStategroupEnabledFn>(0x008302D0)(
                pThing,
                stategroup_name,
                enabled);
        }
    }
}