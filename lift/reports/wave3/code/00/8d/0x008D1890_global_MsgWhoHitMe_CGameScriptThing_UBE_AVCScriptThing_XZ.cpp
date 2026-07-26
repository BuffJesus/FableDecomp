#include <cstddef>
#include <cstdint>

class CBaseObject;
class CBaseIntelligentPointer;
class CScriptThing;
class CGameScriptThing;
class CTCBase;
class CTCEntityEvents;

using ETCInterfaceType = int;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    struct Entry
    {
        TKey first;
        TValue second;
    };

    std::byte m_Pad00[0x04];
    Entry* m_End; // 0x04

    Entry* __thiscall LowerBound(const TKey* key);
};

class CBaseIntelligentPointer
{
public:
    CBaseObject* __thiscall GetPItem() const;
};

class CGameScriptThing
{
public:
    CScriptThing* __thiscall MsgWhoHitMe(CScriptThing* result) const;
};

namespace
{
using MsgWhoHitMe_Map =
    CVectorMap<
        ETCInterfaceType,
        CTCBase*,
        CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct MsgWhoHitMe_ObjectOverlay
{
    std::byte m_Pad00[0x28];
    std::uint8_t m_Flags28; // 0x28
    std::byte m_Pad29[0x1B];
    MsgWhoHitMe_Map m_Field44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(MsgWhoHitMe_ObjectOverlay, m_Flags28) == 0x28);
static_assert(offsetof(MsgWhoHitMe_ObjectOverlay, m_Field44) == 0x44);
static_assert(offsetof(MsgWhoHitMe_ObjectOverlay, m_Flags91) == 0x91);

struct MsgWhoHitMe_GlobalOverlay
{
    std::byte m_Pad00[0x04];
    CScriptThing* m_Field04; // 0x04
    std::byte m_Pad08[0x80];
    void* m_Field88; // 0x88
};
static_assert(offsetof(MsgWhoHitMe_GlobalOverlay, m_Field04) == 0x04);
static_assert(offsetof(MsgWhoHitMe_GlobalOverlay, m_Field88) == 0x88);

struct MsgWhoHitMe_StateOverlayA
{
    std::byte m_Pad00[0x08];
    void* m_Field08; // 0x08
};
static_assert(offsetof(MsgWhoHitMe_StateOverlayA, m_Field08) == 0x08);

struct MsgWhoHitMe_StateOverlayB
{
    std::byte m_Pad00[0x2C];
    void* m_Field2C; // 0x2C
};
static_assert(offsetof(MsgWhoHitMe_StateOverlayB, m_Field2C) == 0x2C);

struct MsgWhoHitMe_StateOverlayC
{
    std::byte m_Pad00[0x18];
    ETCInterfaceType m_Field18; // 0x18
    std::uint32_t m_Field1C; // 0x1C
};
static_assert(offsetof(MsgWhoHitMe_StateOverlayC, m_Field18) == 0x18);
static_assert(offsetof(MsgWhoHitMe_StateOverlayC, m_Field1C) == 0x1C);

struct MsgWhoHitMe_FilterPointers
{
    std::uint8_t* m_LocalC;
    ETCInterfaceType* m_Local8;
    std::uint32_t* m_Local4;
};
static_assert(offsetof(MsgWhoHitMe_FilterPointers, m_LocalC) == 0x00);
static_assert(offsetof(MsgWhoHitMe_FilterPointers, m_Local8) == 0x04);
static_assert(offsetof(MsgWhoHitMe_FilterPointers, m_Local4) == 0x08);

bool __thiscall CTCEntityEvents_GetEvent(
    void* this_,
    void** out_event,
    const MsgWhoHitMe_FilterPointers* filter);

void* __thiscall std__Cons_val(void* this_, CScriptThing* out_result);
void __thiscall NScript_GFPredicateAnd(void* this_);
CScriptThing* __thiscall CScriptThing_CScriptThing(CScriptThing* this_, CScriptThing* out_result);

inline auto& g_GameGlobals =
    *reinterpret_cast<MsgWhoHitMe_GlobalOverlay**>(0x013B89FC);
}

CScriptThing* __thiscall CGameScriptThing::MsgWhoHitMe(CScriptThing* result) const
{
    auto* const item_ptr = reinterpret_cast<CBaseIntelligentPointer*>(
        const_cast<std::byte*>(reinterpret_cast<const std::byte*>(this)) + 0x0C);

    CBaseObject* object = item_ptr->GetPItem();
    if (object != nullptr)
    {
        object = item_ptr->GetPItem();
        const auto* const object_overlay =
            reinterpret_cast<const MsgWhoHitMe_ObjectOverlay*>(object);
        if ((object_overlay->m_Flags91 & 0x01) == 0)
        {
            const auto* const state_a =
                reinterpret_cast<const MsgWhoHitMe_StateOverlayA*>(g_GameGlobals->m_Field88);

            ETCInterfaceType interface_type;
            if ((state_a == nullptr) || (state_a->m_Field08 == nullptr))
            {
                interface_type = 0;
            }
            else
            {
                interface_type =
                    reinterpret_cast<const MsgWhoHitMe_StateOverlayC*>(
                        reinterpret_cast<const MsgWhoHitMe_StateOverlayB*>(state_a->m_Field08)->m_Field2C)
                        ->m_Field18;
            }

            std::uint32_t interface_time;
            if ((state_a == nullptr) || (state_a->m_Field08 == nullptr))
            {
                interface_time = 0;
            }
            else
            {
                interface_time =
                    reinterpret_cast<const MsgWhoHitMe_StateOverlayC*>(
                        reinterpret_cast<const MsgWhoHitMe_StateOverlayB*>(state_a->m_Field08)->m_Field2C)
                        ->m_Field1C;
            }

            object = item_ptr->GetPItem();
            auto* const object_overlay2 =
                reinterpret_cast<MsgWhoHitMe_ObjectOverlay*>(object);
            if ((object_overlay2->m_Flags28 & 0x04) != 0)
            {
                ETCInterfaceType local_18 = 0x42;
                auto* entry = object_overlay2->m_Field44.LowerBound(&local_18);
                if ((entry == object_overlay2->m_Field44.m_End) || (0x42 < entry->first))
                {
                    entry = object_overlay2->m_Field44.m_End;
                }

                std::uint8_t local_19;
                std::uint32_t local_14;
                MsgWhoHitMe_FilterPointers local_c;
                void* local_10;

                local_c.m_LocalC = &local_19;
                local_c.m_Local8 = &local_18;
                local_c.m_Local4 = &local_14;
                local_18 = interface_type;
                local_14 = interface_time;

                if (CTCEntityEvents_GetEvent(
                        reinterpret_cast<void*>(entry->second),
                        &local_10,
                        &local_c))
                {
                    void* const predicate =
                        std__Cons_val(local_10, result);
                    NScript_GFPredicateAnd(predicate);
                    return result;
                }
            }
        }
    }

    CScriptThing_CScriptThing(g_GameGlobals->m_Field04, result);
    return result;
}