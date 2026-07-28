#include <cstddef>
#include <cstdint>

using ETCInterfaceType = int;

struct CTCBase;

struct CTCBase_FindInterfaceType_0x1b_BitsetOverlay {
    std::byte storage[0x24];

    std::uint32_t test(std::size_t index);
};

template <typename K, typename V>
struct CVectorMapEntry {
    K key;
    V value;
};

template <typename K, typename V>
struct CVectorMapOverlay {
    CVectorMapEntry<K, V>* begin;
    CVectorMapEntry<K, V>* end;

    CVectorMapEntry<K, V>* LowerBound(const K* key);
};

struct CTCBase_FindInterfaceType_0x1b_Overlay {
    std::byte pad_00[0x20];
    CTCBase_FindInterfaceType_0x1b_BitsetOverlay interfaceBits;
    CVectorMapOverlay<ETCInterfaceType, CTCBase*> interfaceMap;
};

static_assert(offsetof(CTCBase_FindInterfaceType_0x1b_Overlay, interfaceBits) == 0x20);
static_assert(sizeof(CTCBase_FindInterfaceType_0x1b_BitsetOverlay) == 0x24);
static_assert(offsetof(CTCBase_FindInterfaceType_0x1b_Overlay, interfaceMap) == 0x44);
static_assert(offsetof(CVectorMapOverlay<ETCInterfaceType, CTCBase*>, end) == 0x04);
static_assert(offsetof(CVectorMapEntry<ETCInterfaceType, CTCBase*>, value) == 0x04);

std::uint32_t __thiscall CTCBase_FindInterfaceType_0x1b(CTCBase* self, int* outValue)
{
    auto* const overlay = reinterpret_cast<CTCBase_FindInterfaceType_0x1b_Overlay*>(self);

    ETCInterfaceType key = static_cast<ETCInterfaceType>(reinterpret_cast<std::uintptr_t>(self));
    std::uint32_t result = overlay->interfaceBits.test(0x1b);

    if (static_cast<char>(result) == '\0') {
        result &= 0xFFFFFF00u;
    } else {
        key = 0x1b;

        auto* entry = overlay->interfaceMap.LowerBound(&key);
        if (entry == overlay->interfaceMap.end || 0x1b < entry->key) {
            entry = overlay->interfaceMap.end;
        }

        const std::uint32_t value =
            *reinterpret_cast<const std::uint32_t*>(reinterpret_cast<const std::byte*>(entry) + 0x04);
        *outValue = static_cast<int>(value);
        result = (value & 0xFFFFFF00u) | 0x01u;
    }

    return result;
}