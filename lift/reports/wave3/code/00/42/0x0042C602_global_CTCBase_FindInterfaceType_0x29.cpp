#include <cstddef>
#include <cstdint>

enum ETCInterfaceType : std::uint32_t;
struct CTCBase;

struct CTCBase_InterfaceEntry_Overlay {
    ETCInterfaceType key;
    CTCBase* value;
};

static_assert(sizeof(CTCBase_InterfaceEntry_Overlay) == 0x8);
static_assert(offsetof(CTCBase_InterfaceEntry_Overlay, key) == 0x0);
static_assert(offsetof(CTCBase_InterfaceEntry_Overlay, value) == 0x4);

struct CVectorMap_Interface_Overlay {
    CTCBase_InterfaceEntry_Overlay* begin;
    CTCBase_InterfaceEntry_Overlay* end;
};

static_assert(sizeof(CVectorMap_Interface_Overlay) == 0x8);
static_assert(offsetof(CVectorMap_Interface_Overlay, begin) == 0x0);
static_assert(offsetof(CVectorMap_Interface_Overlay, end) == 0x4);

struct CTCBase_FindInterfaceType_0x29_Overlay {
    std::byte pad_00[0x20];
    std::byte interfaceBitsStorage[0x24];
    CVectorMap_Interface_Overlay interfaceMap;
};

static_assert(offsetof(CTCBase_FindInterfaceType_0x29_Overlay, interfaceBitsStorage) == 0x20);
static_assert(sizeof(CTCBase_FindInterfaceType_0x29_Overlay::interfaceBitsStorage) == 0x24);
static_assert(offsetof(CTCBase_FindInterfaceType_0x29_Overlay, interfaceMap) == 0x44);

extern std::uint32_t __thiscall std__bitset__test(void* bitsetStorage, std::uint32_t bitIndex);

extern CTCBase_InterfaceEntry_Overlay* __thiscall
CVectorMap_ETCInterfaceType_CTCBasePtr_LowerBound(
    CVectorMap_Interface_Overlay* map,
    const ETCInterfaceType* key);

std::uint32_t __thiscall CTCBase_FindInterfaceType_0x29(
    CTCBase_FindInterfaceType_0x29_Overlay* self,
    CTCBase** outInterface)
{
    std::uint32_t localKeyRaw = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self));
    std::uint32_t testResult = std__bitset__test(self->interfaceBitsStorage, 0x29);

    if (static_cast<char>(testResult) == '\0') {
        return testResult & 0xFFFFFF00u;
    }

    localKeyRaw = 0x29;

    CTCBase_InterfaceEntry_Overlay* entry =
        CVectorMap_ETCInterfaceType_CTCBasePtr_LowerBound(
            &self->interfaceMap,
            reinterpret_cast<const ETCInterfaceType*>(&localKeyRaw));

    if (entry == self->interfaceMap.end || static_cast<std::uint32_t>(entry->key) > 0x29u) {
        entry = self->interfaceMap.end;
    }

    CTCBase* value = entry->value;
    *outInterface = value;

    const std::uint32_t valueBits =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(value));
    return (valueBits & 0xFFFFFF00u) | 0x1u;
}