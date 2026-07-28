#include <cstddef>
#include <cstdint>

class CTCBase;

enum ETCInterfaceType : std::uint32_t;

template <typename K, typename V, typename Compare>
class CVectorMap;

template <typename K, typename V>
class CKeyPairCompareLess;

struct CTCBase_FindInterfaceType_0xE8_Overlay
{
    std::byte pad_00[0x20];
    std::byte interfaceTypeBits[0x24];
    std::byte pad_44[0x44 - 0x20 - 0x24];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> interfaceMap;
};

static_assert(offsetof(CTCBase_FindInterfaceType_0xE8_Overlay, interfaceTypeBits) == 0x20);
static_assert(offsetof(CTCBase_FindInterfaceType_0xE8_Overlay, interfaceMap) == 0x44);

bool __thiscall CTCBase_FindInterfaceType_0xe8(CTCBase* this_, CTCBase** outInterface)
{
    auto* const self = reinterpret_cast<CTCBase_FindInterfaceType_0xE8_Overlay*>(this_);

    if (!std__bitset__test(&self->interfaceTypeBits, 0xE8u))
        return false;

    ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0xE8);
    auto* it = self->interfaceMap.LowerBound(&interfaceType);
    if (it == self->interfaceMap.end() || static_cast<std::uint32_t>(0xE8) < static_cast<std::uint32_t>(it->first))
        it = self->interfaceMap.end();

    *outInterface = it->second;
    return true;
}