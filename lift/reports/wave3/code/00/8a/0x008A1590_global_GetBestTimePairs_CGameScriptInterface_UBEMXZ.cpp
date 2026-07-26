#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    float __thiscall GetBestTimePairs() const;
};

enum ETCInterfaceType : std::int32_t;
struct CTCBase;

namespace
{
struct CGameScriptInterface_Overlay
{
    std::byte pad_00[0x14];
    void* field_14;
};
static_assert(offsetof(CGameScriptInterface_Overlay, field_14) == 0x14);

struct CVectorMapEntry_ETCInterfaceType_CTCBasePtr
{
    ETCInterfaceType key;
    CTCBase* value;
};
static_assert(offsetof(CVectorMapEntry_ETCInterfaceType_CTCBasePtr, key) == 0x0);
static_assert(offsetof(CVectorMapEntry_ETCInterfaceType_CTCBasePtr, value) == 0x4);

struct CVectorMap_ETCInterfaceType_CTCBasePtr_Overlay
{
    std::byte pad_00[0x4];
    CVectorMapEntry_ETCInterfaceType_CTCBasePtr* end;
};
static_assert(offsetof(CVectorMap_ETCInterfaceType_CTCBasePtr_Overlay, end) == 0x4);

struct TimePairsSource_Overlay
{
    std::byte pad_00[0x20];
    std::uint8_t flags_20;
    std::byte pad_21[0x44 - 0x21];
    CVectorMap_ETCInterfaceType_CTCBasePtr_Overlay interface_map;
    std::byte pad_4C[0x91 - 0x4C];
    std::uint8_t flags_91;
};
static_assert(offsetof(TimePairsSource_Overlay, flags_20) == 0x20);
static_assert(offsetof(TimePairsSource_Overlay, interface_map) == 0x44);
static_assert(offsetof(TimePairsSource_Overlay, flags_91) == 0x91);

struct CTCBase_Overlay
{
    std::byte pad_00[0x178];
    float field_178;
};
static_assert(offsetof(CTCBase_Overlay, field_178) == 0x178);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DE0 = TimePairsSource_Overlay* (__thiscall*)(void*);
using Fn_00436570 = CVectorMapEntry_ETCInterfaceType_CTCBasePtr* (__thiscall*)(
    CVectorMap_ETCInterfaceType_CTCBasePtr_Overlay*,
    const ETCInterfaceType*);

inline constexpr Fn_00449970 CALL_00449970 = reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DE0 CALL_00487DE0 = reinterpret_cast<Fn_00487DE0>(0x00487DE0);
inline constexpr Fn_00436570 CALL_00436570 = reinterpret_cast<Fn_00436570>(0x00436570);

inline constexpr auto DAT_0122DEDC = reinterpret_cast<const float*>(0x0122DEDC);
}

float __thiscall CGameScriptInterface::GetBestTimePairs() const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    TimePairsSource_Overlay* const source = CALL_00487DE0(CALL_00449970(self->field_14));

    if (source != nullptr && (source->flags_91 & 1u) == 0 && (source->flags_20 & 0x10u) != 0)
    {
        ETCInterfaceType search_key = static_cast<ETCInterfaceType>(4);
        CVectorMapEntry_ETCInterfaceType_CTCBasePtr* entry =
            CALL_00436570(&source->interface_map, &search_key);

        if (entry == source->interface_map.end || static_cast<std::int32_t>(entry->key) > 4)
        {
            entry = source->interface_map.end;
        }

        return reinterpret_cast<const CTCBase_Overlay*>(entry->value)->field_178;
    }

    return *DAT_0122DEDC;
}