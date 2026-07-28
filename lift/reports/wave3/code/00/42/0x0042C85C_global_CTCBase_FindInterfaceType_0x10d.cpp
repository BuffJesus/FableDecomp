class CTCBase;
enum class ETCInterfaceType : unsigned int;

template <typename TKey, typename TValue>
struct CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CVectorMapValuePair {
    TKey first;
    TValue second;
};

template <typename TKey, typename TValue, typename TCompare>
struct CVectorMap {
    using value_type = CVectorMapValuePair<TKey, TValue>;

    value_type* field_0x00;
    value_type* field_0x04;

    value_type* LowerBound(const TKey* key);
};

using CTCBaseInterfaceMap = CVectorMap<
    ETCInterfaceType,
    CTCBase*,
    CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

static_assert(offsetof(CTCBaseInterfaceMap, field_0x04) == 0x4);
static_assert(offsetof(CTCBaseInterfaceMap::value_type, second) == 0x4);

struct CTCBaseInterfaceBitsOverlay {
    std::byte storage[0x24];
};

static_assert(sizeof(CTCBaseInterfaceBitsOverlay) == 0x24);

struct CTCBase_FindInterfaceType_0x10d_Overlay {
    std::byte pad_0x00[0x20];
    CTCBaseInterfaceBitsOverlay interfaceTypeBits;
    CTCBaseInterfaceMap interfaceMap;
};

static_assert(offsetof(CTCBase_FindInterfaceType_0x10d_Overlay, interfaceTypeBits) == 0x20);
static_assert(offsetof(CTCBase_FindInterfaceType_0x10d_Overlay, interfaceMap) == 0x44);

extern "C" unsigned int __thiscall std__bitset__test(
    CTCBaseInterfaceBitsOverlay* self,
    unsigned int bitIndex);

unsigned int __thiscall CTCBase_FindInterfaceType_0x10d(CTCBase* self, CTCBase** outInterface) {
    auto* const overlay = reinterpret_cast<CTCBase_FindInterfaceType_0x10d_Overlay*>(self);
    ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x10D);

    const unsigned int testResult = std__bitset__test(&overlay->interfaceTypeBits, 0x10D);
    if ((testResult & 0xFFu) == 0) {
        return testResult & 0xFFFFFF00u;
    }

    CTCBaseInterfaceMap::value_type* entry = overlay->interfaceMap.LowerBound(&interfaceType);
    if (entry == overlay->interfaceMap.field_0x04 ||
        static_cast<unsigned int>(interfaceType) < static_cast<unsigned int>(entry->first)) {
        entry = overlay->interfaceMap.field_0x04;
    }

    CTCBase* const foundInterface = entry->second;
    *outInterface = foundInterface;

    const unsigned int result = static_cast<unsigned int>(reinterpret_cast<std::uintptr_t>(foundInterface));
    return (result & 0xFFFFFF00u) | 0x01u;
}