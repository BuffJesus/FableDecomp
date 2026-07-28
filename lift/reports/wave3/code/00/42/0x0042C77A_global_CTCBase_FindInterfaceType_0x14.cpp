#include <cstddef>
#include <cstdint>

class CTCBase;
enum ETCInterfaceType : int;

template <typename K, typename V>
struct CKeyPairCompareLess;

template <typename K, typename V, typename Less>
struct CVectorMap;

using CTCBaseInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

// Observed helper call: `lea ecx, [this + 0x20]; push 0x14; call ...`
extern "C" unsigned int __thiscall std__bitset__test(void* bitsetStorage, unsigned int bitIndex);

template <>
struct CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> {
    int* m_pBegin;
    int* m_pEnd;

    int* LowerBound(const ETCInterfaceType* key);
};

static_assert(offsetof(CTCBaseInterfaceMap, m_pEnd) == 0x4);

struct CTCBase_FindInterfaceType_0x14_Overlay {
    std::byte pad_00[0x20];
    std::byte m_InterfaceTypeBits[0x24];
    CTCBaseInterfaceMap m_InterfaceMap;
};

static_assert(offsetof(CTCBase_FindInterfaceType_0x14_Overlay, m_InterfaceTypeBits) == 0x20);
static_assert(offsetof(CTCBase_FindInterfaceType_0x14_Overlay, m_InterfaceMap) == 0x44);

unsigned int __thiscall CTCBase_FindInterfaceType_0x14(CTCBase* self, int* outValue)
{
    auto* const overlay = reinterpret_cast<CTCBase_FindInterfaceType_0x14_Overlay*>(self);

    ETCInterfaceType key = static_cast<ETCInterfaceType>(reinterpret_cast<std::uintptr_t>(self));
    unsigned int result = std__bitset__test(overlay->m_InterfaceTypeBits, 0x14);

    if (static_cast<unsigned char>(result) == 0) {
        result = result & 0xFFFFFF00u;
    } else {
        key = static_cast<ETCInterfaceType>(0x14);

        int* entry = overlay->m_InterfaceMap.LowerBound(&key);
        if (entry == overlay->m_InterfaceMap.m_pEnd || 0x14 < *entry) {
            entry = overlay->m_InterfaceMap.m_pEnd;
        }

        const int value = entry[1];
        *outValue = value;
        result = (static_cast<unsigned int>(value) & 0xFFFFFF00u) | 1u;
    }

    return result;
}