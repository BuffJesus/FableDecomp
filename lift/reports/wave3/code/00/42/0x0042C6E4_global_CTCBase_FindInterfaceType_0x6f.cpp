#include <bitset>
#include <cstddef>
#include <cstdint>

using ETCInterfaceType = std::uint32_t;

struct CTCBase;

struct CTCBaseInterfaceMapEntry {
    ETCInterfaceType key;
    CTCBase* value;
};
static_assert(offsetof(CTCBaseInterfaceMapEntry, key) == 0x0);
static_assert(offsetof(CTCBaseInterfaceMapEntry, value) == 0x4);
static_assert(sizeof(CTCBaseInterfaceMapEntry) == 0x8);

struct CTCBaseInterfaceMapOverlay {
    CTCBaseInterfaceMapEntry* begin;
    CTCBaseInterfaceMapEntry* end;

    CTCBaseInterfaceMapEntry* __thiscall LowerBound(const ETCInterfaceType* key);
};
static_assert(offsetof(CTCBaseInterfaceMapOverlay, begin) == 0x0);
static_assert(offsetof(CTCBaseInterfaceMapOverlay, end) == 0x4);
static_assert(sizeof(CTCBaseInterfaceMapOverlay) == 0x8);

struct CTCBaseFindInterfaceType0x6FOverlay {
    std::byte pad_00[0x20];
    std::bitset<0x70> interfaceBits;
    CTCBaseInterfaceMapOverlay interfaceMap;
};
static_assert(offsetof(CTCBaseFindInterfaceType0x6FOverlay, interfaceBits) == 0x20);
static_assert(offsetof(CTCBaseFindInterfaceType0x6FOverlay, interfaceMap) == 0x44);

extern "C" std::uint32_t __thiscall std__bitset__test(
    std::bitset<0x70>* bitset,
    ETCInterfaceType index);

std::uint32_t __thiscall CTCBase_FindInterfaceType_0x6f(
    void* self,
    CTCBase** outInterface)
{
    auto* const overlay = static_cast<CTCBaseFindInterfaceType0x6FOverlay*>(self);
    ETCInterfaceType interfaceType = 0x6Fu;

    const std::uint32_t testResult = std__bitset__test(&overlay->interfaceBits, 0x6Fu);
    if ((testResult & 0xFFu) == 0) {
        return testResult & 0xFFFFFF00u;
    }

    CTCBaseInterfaceMapEntry* entry = overlay->interfaceMap.LowerBound(&interfaceType);
    if ((entry == overlay->interfaceMap.end) || (0x6Fu < entry->key)) {
        entry = overlay->interfaceMap.end;
    }

    CTCBase* const found = entry->value;
    *outInterface = found;

    const std::uint32_t foundAsUint =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(found));
    return (foundAsUint & 0xFFFFFF00u) | 1u;
}