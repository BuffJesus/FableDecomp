#include <bitset>
#include <cstddef>
#include <cstdint>

enum ETCInterfaceType : std::uint32_t;
struct CTCBase;

template <typename K, typename V>
struct CKeyPairCompareLess;

template <typename K, typename V>
struct CVectorMapPair {
    K key;
    V value;
};

static_assert(sizeof(CVectorMapPair<ETCInterfaceType, CTCBase*>) == 0x8);

template <typename K, typename V, typename Less>
struct CVectorMap {
    CVectorMapPair<K, V>* m_begin;
    CVectorMapPair<K, V>* m_end;

    auto LowerBound(const K* key) -> CVectorMapPair<K, V>*;
};

using CTCBaseInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

static_assert(offsetof(CTCBaseInterfaceMap, m_end) == 0x4);

struct CTCBase_FindInterfaceType_0xfc_Overlay {
    std::byte pad_00[0x20];
    std::bitset<0x120> interfaceTypeBits;
    CTCBaseInterfaceMap interfaceMap;
};

static_assert(offsetof(CTCBase_FindInterfaceType_0xfc_Overlay, interfaceTypeBits) == 0x20);
static_assert(offsetof(CTCBase_FindInterfaceType_0xfc_Overlay, interfaceMap) == 0x44);

// Matches the observed helper call shape/dataflow: callers consume the full EAX value
// and branch only on its low byte.
extern std::uint32_t __thiscall std__bitset__test(std::bitset<0x120>* self, std::uint32_t index);

std::uint32_t __thiscall CTCBase_FindInterfaceType_0xfc(CTCBase* self, int* outValue)
{
    auto* const overlay = reinterpret_cast<CTCBase_FindInterfaceType_0xfc_Overlay*>(self);

    ETCInterfaceType key = static_cast<ETCInterfaceType>(0xFC);

    std::uint32_t testResult = std__bitset__test(&overlay->interfaceTypeBits, 0xFC);
    if (static_cast<unsigned char>(testResult) == 0) {
        return testResult & 0xFFFFFF00u;
    }

    key = static_cast<ETCInterfaceType>(0xFC);

    auto* pair = overlay->interfaceMap.LowerBound(&key);
    if ((pair == overlay->interfaceMap.m_end) || (static_cast<ETCInterfaceType>(0xFC) < pair->key)) {
        pair = overlay->interfaceMap.m_end;
    }

    const int value = reinterpret_cast<int>(pair->value);
    *outValue = value;
    return (static_cast<std::uint32_t>(value) & 0xFFFFFF00u) | 1u;
}