#include <cstddef>
#include <cstdint>

class C3DVector;
class CDefinitionManager;
class CGameScriptInterface;
class CRightHandedSet;
class CScriptThing;
class CTCBase;
class CTCCarrying;
class CThing;
class CVertexBufferWin32;

enum ETCInterfaceType : std::int32_t
{
};

template <typename T>
class CBasicString;

template <>
class CBasicString<char>
{
public:
    bool __thiscall operator==(const char* string) const;
};

class CCharString
{
public:
    CBasicString<char>* m_pBasicString; // 0x00
    std::int32_t m_Unknown04;           // 0x04

    CCharString(const char* string, long length);
    ~CCharString();
};

static_assert(offsetof(CCharString, m_pBasicString) == 0x00);
static_assert(sizeof(CCharString) == 0x08);

class CDefinitionManager
{
public:
    long __thiscall GetDefGlobalIndexFromName(const CCharString* name);
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;       // 0x00
    TValue m_Value;   // 0x04
};

static_assert(sizeof(CKeyPair<ETCInterfaceType, CTCBase*>) == 0x08);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey& key);
};

using CThingInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

class CThing
{
public:
    void __thiscall Kill(bool destroy_immediately);
    void __thiscall SetInLimbo(bool in_limbo);
};

class CVertexBufferWin32
{
public:
    C3DVector* __thiscall DoSizeof();
};

class CThingObject
{
public:
    static CThing* Create(
        long definition_index,
        C3DVector* position,
        int layer,
        CRightHandedSet* handed_set,
        unsigned long zero,
        CCharString* name);
};

class CTCCarrying
{
public:
    CThing* __thiscall RemoveThingInCarrySlot(long slot_id, bool keep_thing);
    bool __thiscall IsSlotPrimarySlot(long slot_id);
    void __thiscall RemoveThingFromPrimarySlot(bool keep_thing);
    void __thiscall RemoveThing(CThing* thing);
    bool __thiscall IsCarryingThing(CThing* thing);
    void __thiscall PutThingInCarrySlot(CThing* thing, long slot_id, bool update);
};

CThing* __cdecl CThing_Array_FindByID(long thing_id);
char __cdecl InventoryFindItemByIDLowerBound(long item_id);

class CGameScriptInterface
{
public:
    void __thiscall GiveThingItemInSlot(
        const CScriptThing& thing,
        const CCharString& item_type,
        const CCharString& slot_name) const;
};

namespace
{
using CScriptThing_Slot2C_t = CVertexBufferWin32*(__thiscall*)(const CScriptThing* this_ptr);

struct CScriptThingOverlay
{
    void** m_Vftable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_Vftable) == 0x00);

struct CGameScriptInterfaceOverlay
{
    std::byte m_Pad00[0x10];
    CDefinitionManager* m_DefinitionManager; // 0x10
};

static_assert(offsetof(CGameScriptInterfaceOverlay, m_DefinitionManager) == 0x10);

struct CThingOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20;    // 0x20
    std::byte m_Pad24[0x04];
    std::uint32_t m_Flags28;    // 0x28
    std::byte m_Pad2C[0x18];
    CThingInterfaceMap m_Map44; // 0x44
    std::byte m_Pad4C[0x44];
    std::int8_t m_Layer90;      // 0x90
    std::uint8_t m_Flags91;     // 0x91
};

static_assert(offsetof(CThingOverlay, m_Flags20) == 0x20);
static_assert(offsetof(CThingOverlay, m_Flags28) == 0x28);
static_assert(offsetof(CThingOverlay, m_Map44) == 0x44);
static_assert(offsetof(CThingOverlay, m_Layer90) == 0x90);
static_assert(offsetof(CThingOverlay, m_Flags91) == 0x91);

struct CTCStateOverlay
{
    std::byte m_Pad00[0x10];
    std::int32_t m_Field10; // 0x10
};

static_assert(offsetof(CTCStateOverlay, m_Field10) == 0x10);
}

void __thiscall CGameScriptInterface::GiveThingItemInSlot(
    const CScriptThing& thing,
    const CCharString& item_type,
    const CCharString& slot_name) const
{
    const auto* const script_thing = reinterpret_cast<const CScriptThingOverlay*>(&thing);
    const auto get_thing =
        reinterpret_cast<CScriptThing_Slot2C_t>(script_thing->m_Vftable[0x2C / sizeof(void*)]);

    CVertexBufferWin32* const raw_thing = get_thing(&thing);
    if (raw_thing == nullptr)
    {
        return;
    }

    auto* const thing_overlay = reinterpret_cast<CThingOverlay*>(raw_thing);

    bool item_type_is_empty;
    if (item_type.m_pBasicString == nullptr)
    {
        item_type_is_empty = true;
    }
    else
    {
        item_type_is_empty = item_type.m_pBasicString->operator==("");
    }

    const auto* const interface_overlay =
        reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);

    if (item_type_is_empty)
    {
        if ((thing_overlay->m_Flags28 & 0x40u) != 0)
        {
            const auto carrying_type = static_cast<ETCInterfaceType>(0x46);
            CKeyPair<ETCInterfaceType, CTCBase*>* carrying_it =
                thing_overlay->m_Map44.LowerBound(carrying_type);
            if ((carrying_it == thing_overlay->m_Map44.m_pEnd) ||
                (carrying_type < carrying_it->m_Key))
            {
                carrying_it = thing_overlay->m_Map44.m_pEnd;
            }

            auto* const carrying = reinterpret_cast<CTCCarrying*>(carrying_it->m_Value);
            const long slot_id =
                interface_overlay->m_DefinitionManager->GetDefGlobalIndexFromName(&slot_name);

            if (slot_id > 0)
            {
                CThing* const removed_thing = carrying->RemoveThingInCarrySlot(slot_id, false);
                if ((removed_thing != nullptr) &&
                    ((reinterpret_cast<CThingOverlay*>(removed_thing)->m_Flags91 & 0x01u) == 0))
                {
                    removed_thing->Kill(false);
                    return;
                }
            }
        }

        return;
    }

    const long definition_index =
        interface_overlay->m_DefinitionManager->GetDefGlobalIndexFromName(&item_type);
    if ((definition_index <= 0) || ((thing_overlay->m_Flags28 & 0x40u) == 0))
    {
        return;
    }

    const auto carrying_type = static_cast<ETCInterfaceType>(0x46);
    CKeyPair<ETCInterfaceType, CTCBase*>* carrying_it =
        thing_overlay->m_Map44.LowerBound(carrying_type);
    if ((carrying_it == thing_overlay->m_Map44.m_pEnd) ||
        (carrying_type < carrying_it->m_Key))
    {
        carrying_it = thing_overlay->m_Map44.m_pEnd;
    }

    auto* const carrying = reinterpret_cast<CTCCarrying*>(carrying_it->m_Value);

    CThing* created_thing;
    {
        CCharString empty_name("", -1);
        const int layer = static_cast<int>(thing_overlay->m_Layer90);
        CRightHandedSet* const handed_set = nullptr;
        const unsigned long zero = 0;

        created_thing = CThingObject::Create(
            definition_index,
            raw_thing->DoSizeof(),
            layer,
            handed_set,
            zero,
            &empty_name);

        // Ghidra shows a direct call immediately after Create; this scope models that exact
        // compiler-emitted cleanup as the single CCharString destructor call for empty_name.
    }

    const long slot_id =
        interface_overlay->m_DefinitionManager->GetDefGlobalIndexFromName(&slot_name);
    if (slot_id > 0)
    {
        if (InventoryFindItemByIDLowerBound(slot_id) == '\0')
        {
            if (carrying->IsSlotPrimarySlot(slot_id))
            {
                carrying->RemoveThingFromPrimarySlot(false);
            }
            else
            {
                CThing* const existing_thing = CThing_Array_FindByID(slot_id);
                carrying->RemoveThing(existing_thing);

                if ((!carrying->IsCarryingThing(existing_thing)) &&
                    ((reinterpret_cast<CThingOverlay*>(existing_thing)->m_Flags20 & 0x04000000u) != 0))
                {
                    auto* const existing_overlay = reinterpret_cast<CThingOverlay*>(existing_thing);
                    const auto state_type = static_cast<ETCInterfaceType>(0x1A);
                    CKeyPair<ETCInterfaceType, CTCBase*>* state_it =
                        existing_overlay->m_Map44.LowerBound(state_type);
                    if ((state_it == existing_overlay->m_Map44.m_pEnd) ||
                        (state_type < state_it->m_Key))
                    {
                        state_it = existing_overlay->m_Map44.m_pEnd;
                    }

                    if (reinterpret_cast<CTCStateOverlay*>(state_it->m_Value)->m_Field10 != 0)
                    {
                        existing_thing->SetInLimbo(true);
                    }
                }
            }
        }

        carrying->PutThingInCarrySlot(created_thing, slot_id, true);
    }
}