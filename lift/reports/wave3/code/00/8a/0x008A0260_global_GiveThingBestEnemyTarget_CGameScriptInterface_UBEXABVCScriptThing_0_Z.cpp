#include <cstddef>
#include <cstdint>
#include <new>

class CCharString;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CThing;
class CVillageTaskCarryCrate;

enum ETCInterfaceType : std::int32_t;

template <typename T>
class CBasicString;

template <>
class CBasicString<char>
{
public:
    static long __cdecl Compare(const char* lhs, const char* rhs);
};

class CCharString
{
public:
    char* m_pString;          // 0x00
    std::int32_t m_Length;    // 0x04

    CCharString(const char* string, std::int32_t length);
    ~CCharString();
};

static_assert(offsetof(CCharString, m_pString) == 0x00);
static_assert(offsetof(CCharString, m_Length) == 0x04);
static_assert(sizeof(CCharString) == 0x08);

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;       // 0x00
    TValue m_Value;   // 0x04
};

static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Key) == 0x00);
static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Value) == 0x04);
static_assert(sizeof(CKeyPair<ETCInterfaceType, CTCBase*>) == 0x08);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* LowerBound(const TKey& key);
};

static_assert(offsetof(CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>, m_pBegin) == 0x00);
static_assert(offsetof(CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>, m_pEnd) == 0x04);

using CScriptThing_Slot2C_t = CThing* (__thiscall*)(const CScriptThing* this_ptr);
using CScriptThing_Slot12C_t = char (__thiscall*)(const CScriptThing* this_ptr);

struct CScriptThing_vftable
{
    std::byte m_Pad0[0x2C];
    CScriptThing_Slot2C_t m_Slot2C;       // 0x2C
    std::byte m_Pad30[0x12C - 0x30];
    CScriptThing_Slot12C_t m_Slot12C;     // 0x12C
};

static_assert(offsetof(CScriptThing_vftable, m_Slot2C) == 0x2C);
static_assert(offsetof(CScriptThing_vftable, m_Slot12C) == 0x12C);

using CTCBase_Slot58_t = CCharString* (__thiscall*)(CTCBase* this_ptr, CCharString* out_string);
using CVillageTaskCarryCrate_Slot74_t = CThing* (__thiscall*)(CVillageTaskCarryCrate* this_ptr);

struct CVillageTaskCarryCrate_vftable
{
    std::byte m_Pad0[0x58];
    CTCBase_Slot58_t m_Slot58;                // 0x58
    std::byte m_Pad5C[0x74 - 0x5C];
    CVillageTaskCarryCrate_Slot74_t m_Slot74; // 0x74
};

static_assert(offsetof(CVillageTaskCarryCrate_vftable, m_Slot58) == 0x58);
static_assert(offsetof(CVillageTaskCarryCrate_vftable, m_Slot74) == 0x74);

class CVillageTaskCarryCrate
{
public:
    void AssignCreatureToTask(CThing* creature);
};

using CThing_InterfaceMap_t =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CThingOverlay
{
    std::byte m_Pad0[0x10];
    std::uint8_t m_Byte10;          // 0x10
    std::byte m_Pad11[0x20 - 0x11];
    std::uint32_t m_Flags20;        // 0x20
    std::byte m_Pad24[0x28 - 0x24];
    std::uint32_t m_Flags28;        // 0x28
    std::byte m_Pad2C[0x44 - 0x2C];
    CThing_InterfaceMap_t m_Interfaces; // 0x44
    std::byte m_Pad4C[0x6C - 0x4C];
    std::uint8_t m_Byte6C;          // 0x6C
};

static_assert(offsetof(CThingOverlay, m_Byte10) == 0x10);
static_assert(offsetof(CThingOverlay, m_Flags20) == 0x20);
static_assert(offsetof(CThingOverlay, m_Flags28) == 0x28);
static_assert(offsetof(CThingOverlay, m_Interfaces) == 0x44);
static_assert(offsetof(CThingOverlay, m_Byte6C) == 0x6C);

void __thiscall CCreature_AddEnemyToList(CTCBase* this_ptr, CThing* enemy);

class CGameScriptInterface
{
public:
    void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& best_enemy_target) const;
};

void CGameScriptInterface::GiveThingBestEnemyTarget(
    const CScriptThing& thing,
    const CScriptThing& best_enemy_target) const
{
    const auto* const thing_vftable = *reinterpret_cast<CScriptThing_vftable* const*>(&thing);
    if (thing_vftable->m_Slot12C(&thing) == '\0')
    {
        return;
    }

    CThing* previous_task_creature = nullptr;

    CThing* const raw_thing = thing_vftable->m_Slot2C(&thing);
    auto* const thing_overlay = reinterpret_cast<CThingOverlay*>(raw_thing);

    if ((thing_overlay->m_Flags20 & 0x100u) != 0)
    {
        const auto targeting_ai_type = static_cast<ETCInterfaceType>(0x08);
        CKeyPair<ETCInterfaceType, CTCBase*>* targeting_ai_it =
            thing_overlay->m_Interfaces.LowerBound(targeting_ai_type);

        if ((targeting_ai_it == thing_overlay->m_Interfaces.m_pEnd) || (targeting_ai_it->m_Key > targeting_ai_type))
        {
            targeting_ai_it = thing_overlay->m_Interfaces.m_pEnd;
        }

        auto* const targeting_ai = static_cast<CVillageTaskCarryCrate*>(targeting_ai_it->m_Value);
        const auto* const targeting_ai_vftable =
            *reinterpret_cast<CVillageTaskCarryCrate_vftable* const*>(targeting_ai);

        alignas(CCharString) std::byte type_name_storage[sizeof(CCharString)];
        alignas(CCharString) std::byte expected_name_storage[sizeof(CCharString)];

        auto* const type_name = reinterpret_cast<CCharString*>(type_name_storage);
        auto* const expected_name = new (expected_name_storage) CCharString("CTCTargetingAI", -1);

        CCharString* const returned_type_name = targeting_ai_vftable->m_Slot58(
            reinterpret_cast<CTCBase*>(targeting_ai),
            type_name);

        bool names_match;
        if (returned_type_name == expected_name)
        {
            names_match = true;
        }
        else if ((returned_type_name == nullptr) || (expected_name == nullptr))
        {
            names_match = false;
        }
        else if (returned_type_name->m_Length == expected_name->m_Length)
        {
            names_match =
                CBasicString<char>::Compare(returned_type_name->m_pString, expected_name->m_pString) == 0;
        }
        else
        {
            names_match = false;
        }

        type_name->~CCharString();
        expected_name->~CCharString();

        if (names_match)
        {
            previous_task_creature = targeting_ai_vftable->m_Slot74(targeting_ai);

            const auto* const best_enemy_target_vftable =
                *reinterpret_cast<CScriptThing_vftable* const*>(&best_enemy_target);

            CThing* target_creature;
            if (best_enemy_target_vftable->m_Slot12C(&best_enemy_target) == '\0')
            {
                target_creature = nullptr;
            }
            else
            {
                CThing* const raw_best_enemy_target = best_enemy_target_vftable->m_Slot2C(&best_enemy_target);
                if ((reinterpret_cast<CThingOverlay*>(raw_best_enemy_target)->m_Byte6C & 0x08u) == 0)
                {
                    target_creature = nullptr;
                }
                else
                {
                    target_creature = best_enemy_target_vftable->m_Slot2C(&best_enemy_target);
                }
            }

            targeting_ai->AssignCreatureToTask(target_creature);
        }
    }

    const auto* const best_enemy_target_vftable = *reinterpret_cast<CScriptThing_vftable* const*>(&best_enemy_target);
    if (best_enemy_target_vftable->m_Slot12C(&best_enemy_target) != '\0')
    {
        CThing* const raw_thing_again = thing_vftable->m_Slot2C(&thing);
        auto* const thing_overlay_again = reinterpret_cast<CThingOverlay*>(raw_thing_again);

        if ((thing_overlay_again->m_Flags28 & 0x200u) != 0)
        {
            const auto add_enemy_interface_type = static_cast<ETCInterfaceType>(0x49);
            CKeyPair<ETCInterfaceType, CTCBase*>* const add_enemy_it =
                thing_overlay_again->m_Interfaces.LowerBound(add_enemy_interface_type);

            CCreature_AddEnemyToList(
                add_enemy_it->m_Value,
                best_enemy_target_vftable->m_Slot2C(&best_enemy_target));
        }
    }

    if (previous_task_creature != best_enemy_target_vftable->m_Slot2C(&best_enemy_target))
    {
        CThing* const raw_thing_final = thing_vftable->m_Slot2C(&thing);
        if (reinterpret_cast<CThingOverlay*>(raw_thing_final)->m_Byte10 == 1)
        {
            reinterpret_cast<void(__thiscall*)(CThing*)>(0x006D5930)(thing_vftable->m_Slot2C(&thing));
        }
    }
}