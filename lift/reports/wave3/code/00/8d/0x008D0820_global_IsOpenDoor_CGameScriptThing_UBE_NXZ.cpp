#include <cstddef>
#include <cstdint>

using ETCInterfaceType = int;

class CBaseObject;
class CTCBase;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

class CBaseIntelligentPointer
{
public:
    CBaseObject* GetPItem() const;
};

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

    Entry* LowerBound(const TKey* key);
};

class CGameScriptThing
{
public:
    bool __thiscall IsOpenDoor() const;
};

namespace
{
using CGameScriptThing_IsOpenDoor_Map =
    CVectorMap<
        ETCInterfaceType,
        CTCBase*,
        CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptThing_IsOpenDoor_ThisOverlay
{
    std::byte m_Pad00[0x0C];
};
static_assert(sizeof(CGameScriptThing_IsOpenDoor_ThisOverlay) == 0x0C);

static_assert(offsetof(CGameScriptThing_IsOpenDoor_Map, m_End) == 0x04);

struct CGameScriptThing_IsOpenDoor_ObjectOverlay
{
    std::byte m_Pad00[0x24];
    std::uint8_t m_Field24; // 0x24
    std::byte m_Pad25[0x1F];
    CGameScriptThing_IsOpenDoor_Map m_Field44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Field91; // 0x91
};
static_assert(offsetof(CGameScriptThing_IsOpenDoor_ObjectOverlay, m_Field24) == 0x24);
static_assert(offsetof(CGameScriptThing_IsOpenDoor_ObjectOverlay, m_Field44) == 0x44);
static_assert(offsetof(CGameScriptThing_IsOpenDoor_ObjectOverlay, m_Field91) == 0x91);

struct CGameScriptThing_IsOpenDoor_StateOverlay
{
    std::byte m_Pad00[0x10];
    char m_Field10; // 0x10
    std::byte m_Field11; // 0x11
    char m_Field12; // 0x12
    char m_Field13; // 0x13
};
static_assert(offsetof(CGameScriptThing_IsOpenDoor_StateOverlay, m_Field10) == 0x10);
static_assert(offsetof(CGameScriptThing_IsOpenDoor_StateOverlay, m_Field12) == 0x12);
static_assert(offsetof(CGameScriptThing_IsOpenDoor_StateOverlay, m_Field13) == 0x13);
}

bool __thiscall CGameScriptThing::IsOpenDoor() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptThing_IsOpenDoor_ThisOverlay*>(this);
    auto* const intelligentPointer = reinterpret_cast<CBaseIntelligentPointer*>(
        const_cast<std::byte*>(reinterpret_cast<const std::byte*>(self)) + 0x0C);

    CBaseObject* item = intelligentPointer->GetPItem();
    CBaseObject* object = nullptr;

    if (item != nullptr)
    {
        object = intelligentPointer->GetPItem();
        const auto* const object91 =
            reinterpret_cast<const CGameScriptThing_IsOpenDoor_ObjectOverlay*>(object);

        if ((object91->m_Field91 & 0x01u) == 0)
        {
            object = intelligentPointer->GetPItem();
            auto* const object24 =
                reinterpret_cast<CGameScriptThing_IsOpenDoor_ObjectOverlay*>(object);

            if ((object24->m_Field24 & 0x40u) != 0)
            {
                ETCInterfaceType key = static_cast<ETCInterfaceType>(0x26);
                auto* entry = object24->m_Field44.LowerBound(&key);

                if ((entry == object24->m_Field44.m_End) || (static_cast<ETCInterfaceType>(0x26) < entry->first))
                {
                    entry = object24->m_Field44.m_End;
                }

                const auto* const state =
                    reinterpret_cast<const CGameScriptThing_IsOpenDoor_StateOverlay*>(entry->second);

                if ((state->m_Field12 == '\0') &&
                    (state->m_Field13 == '\0') &&
                    (state->m_Field10 != '\0'))
                {
                    return true;
                }

                return false;
            }
        }
    }

    return false;
}