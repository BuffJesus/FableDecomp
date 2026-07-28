#include <bit>
#include <bitset>
#include <cstddef>
#include <cstdint>

template <typename TKey, typename TValue>
struct CVectorMap_0x13_Overlay {
    struct Entry {
        TKey key;
        TValue value;
    };

    Entry* begin;
    Entry* end;

    Entry* LowerBound(const TKey* key) {
        Entry* first = begin;
        Entry* last = end;
        while (first < last) {
            Entry* const mid = first + ((last - first) >> 1);
            if (mid->key < *key) {
                first = mid + 1;
            } else {
                last = mid;
            }
        }
        return first;
    }
};

static_assert(offsetof(CVectorMap_0x13_Overlay<ETCInterfaceType, CTCBase*>::Entry, key) == 0x0);
static_assert(offsetof(CVectorMap_0x13_Overlay<ETCInterfaceType, CTCBase*>::Entry, value) == 0x4);
static_assert(offsetof(CVectorMap_0x13_Overlay<ETCInterfaceType, CTCBase*>, begin) == 0x0);
static_assert(offsetof(CVectorMap_0x13_Overlay<ETCInterfaceType, CTCBase*>, end) == 0x4);

struct CTCBase_FindInterfaceType_0x13_Overlay {
    std::byte pad_00[0x20];
    std::bitset<32> field_20;
    std::byte pad_24[0x20];
    CVectorMap_0x13_Overlay<ETCInterfaceType, CTCBase*> field_44;
};

static_assert(offsetof(CTCBase_FindInterfaceType_0x13_Overlay, field_20) == 0x20);
static_assert(offsetof(CTCBase_FindInterfaceType_0x13_Overlay, field_44) == 0x44);

extern "C" unsigned int __thiscall std__bitset__test(const void* bitset, unsigned int bitIndex);

unsigned int __thiscall CTCBase_FindInterfaceType_0x13(CTCBase* self, CTCBase** outInterface) {
    auto* const overlay = reinterpret_cast<CTCBase_FindInterfaceType_0x13_Overlay*>(self);

    unsigned int testResult = std__bitset__test(&overlay->field_20, 0x13);
    if (static_cast<std::uint8_t>(testResult) == 0) {
        return testResult & 0xFFFFFF00u;
    }

    ETCInterfaceType lookupKey = static_cast<ETCInterfaceType>(0x13);
    auto* entry = overlay->field_44.LowerBound(&lookupKey);
    if (entry == overlay->field_44.end || static_cast<unsigned int>(entry->key) > 0x13u) {
        entry = overlay->field_44.end;
    }

    CTCBase* const found = entry->value;
    *outInterface = found;

    const std::uint32_t foundBits = std::bit_cast<std::uint32_t>(found);
    return (foundBits & 0xFFFFFF00u) | 1u;
}