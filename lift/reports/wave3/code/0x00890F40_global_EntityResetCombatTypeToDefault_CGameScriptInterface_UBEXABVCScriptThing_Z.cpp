#include <cstddef>
#include <cstdint>

class CDisplayEngine;
class CGameScriptInterface;
class CScriptThing;
class CThing;

namespace NDisplayView
{
class CViewBase
{
public:
    CDisplayEngine* __thiscall GetDisplayEngine();
};
}

class CGlobalDefNameStringKeyOverlay
{
public:
    const void* m_pStringKey; // 0x00
};

static_assert(offsetof(CGlobalDefNameStringKeyOverlay, m_pStringKey) == 0x00);
static_assert(sizeof(CGlobalDefNameStringKeyOverlay) == 0x04);

class CGlobalDefNameOverlay
{
public:
    CGlobalDefNameStringKeyOverlay* __thiscall GetStringKeyOverlay();
};

class CDefinitionManager
{
public:
    CGlobalDefNameOverlay* __thiscall GetDefNameFromGlobalIndex(
        const CGameScriptInterface* const* pGlobalIndexContext);
};

class CDefStringHandle
{
public:
    void* m_pValue; // 0x00

    ~CDefStringHandle();
};

static_assert(offsetof(CDefStringHandle, m_pValue) == 0x00);
static_assert(sizeof(CDefStringHandle) == 0x04);

class CDefStringTable
{
public:
    void __thiscall GetString(CDefStringHandle* pOutString, const void* pStringKey);
};

extern CDefStringTable DAT_013ca828;

class CThing
{
public:
    void __thiscall CountReferences();
};

class CGameScriptInterface
{
public:
    void __thiscall EntityResetCombatTypeToDefault(const CScriptThing& entity) const;
};

namespace
{
struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CGameScriptInterfaceVTableOverlay
{
    std::byte m_Pad00[0x734];
    void (__thiscall* Call0x734)(
        const CGameScriptInterface* pThis,
        const CScriptThing& entity,
        const CDefStringHandle& combatType); // 0x734
};

static_assert(offsetof(CGameScriptInterfaceVTableOverlay, Call0x734) == 0x734);

struct CGameScriptInterfaceOverlay
{
    const CGameScriptInterfaceVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_pVTable) == 0x00);

struct CThingCombatOverlay
{
    std::byte m_Pad00[0x6C];
    std::uint8_t m_Flags6C; // 0x6C
    std::byte m_Pad6D[0x24];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThingCombatOverlay, m_Flags6C) == 0x6C);
static_assert(offsetof(CThingCombatOverlay, m_Flags91) == 0x91);
}

void __thiscall CGameScriptInterface::EntityResetCombatTypeToDefault(const CScriptThing& entity) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    CThing* const pThing = entityOverlay->m_pVTable->GetThing(&entity);

    if (pThing != nullptr)
    {
        const auto* const pThingOverlay = reinterpret_cast<const CThingCombatOverlay*>(pThing);

        if (((pThingOverlay->m_Flags91 & 0x01U) == 0U) &&
            ((pThingOverlay->m_Flags6C & 0x08U) != 0U))
        {
            const CGameScriptInterface* thisCopy = this;
            pThing->CountReferences();

            CDefStringHandle combatType;
            CGlobalDefNameOverlay* const defName =
                reinterpret_cast<CDefinitionManager*>(
                    reinterpret_cast<NDisplayView::CViewBase*>(pThing)->GetDisplayEngine())
                    ->GetDefNameFromGlobalIndex(&thisCopy);
            CGlobalDefNameStringKeyOverlay* const stringKeyOverlay =
                defName->GetStringKeyOverlay();
            DAT_013ca828.GetString(&combatType, stringKeyOverlay->m_pStringKey);

            const auto* const gameScriptInterfaceOverlay =
                reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
            gameScriptInterfaceOverlay->m_pVTable->Call0x734(this, entity, combatType);
        }
    }
}