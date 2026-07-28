#include <cstddef>
#include <cstdint>

struct CTCBase;
enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
struct CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap {
public:
    void* __thiscall LowerBound(const TKey* key);
};

using CTCBaseInterfaceMap = CVectorMap<
    ETCInterfaceType,
    CTCBase*,
    CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CTCBase_FindInterfaceType_0x12_MapEntry {
    ETCInterfaceType key;
    CTCBase* value;
};
static_assert(offsetof(CTCBase_FindInterfaceType_0x12_MapEntry, key) == 0x0);
static_assert(offsetof(CTCBase_FindInterfaceType_0x12_MapEntry, value) == 0x4);
static_assert(sizeof(CTCBase_FindInterfaceType_0x12_MapEntry) == 0x8);

struct CTCBase_FindInterfaceType_0x12_MapOverlay {
    CTCBase_FindInterfaceType_0x12_MapEntry* begin;
    CTCBase_FindInterfaceType_0x12_MapEntry* end;
};
static_assert(offsetof(CTCBase_FindInterfaceType_0x12_MapOverlay, begin) == 0x0);
static_assert(offsetof(CTCBase_FindInterfaceType_0x12_MapOverlay, end) == 0x4);
static_assert(sizeof(CTCBase_FindInterfaceType_0x12_MapOverlay) == 0x8);

struct CTCBase_FindInterfaceType_0x12_BitsetOverlay {
    std::byte storage[0x24];

    std::uint32_t __thiscall test(std::size_t bitIndex) const;
};
static_assert(sizeof(CTCBase_FindInterfaceType_0x12_BitsetOverlay) == 0x24);

struct CTCBase_FindInterfaceType_0x12_Overlay {
    std::byte pad_00[0x20];
    CTCBase_FindInterfaceType_0x12_BitsetOverlay interfaceTypeBitset;
    CTCBase_FindInterfaceType_0x12_MapOverlay interfaceMap;
};
static_assert(offsetof(CTCBase_FindInterfaceType_0x12_Overlay, interfaceTypeBitset) == 0x20);
static_assert(offsetof(CTCBase_FindInterfaceType_0x12_Overlay, interfaceMap) == 0x44);

std::uint32_t __thiscall CTCBase_FindInterfaceType_0x12(CTCBase* self, CTCBase** outInterface)
{
    auto* const overlay = reinterpret_cast<CTCBase_FindInterfaceType_0x12_Overlay*>(self);

    ETCInterfaceType local_8 = static_cast<ETCInterfaceType>(reinterpret_cast<std::uintptr_t>(self));
    std::uint32_t uVar2 = overlay->interfaceTypeBitset.test(0x12);
    if ((static_cast<char>(uVar2)) == '\0') {
        uVar2 = uVar2 & 0xFFFFFF00u;
    } else {
        local_8 = static_cast<ETCInterfaceType>(0x12);
        auto* piVar3 = static_cast<CTCBase_FindInterfaceType_0x12_MapEntry*>(
            reinterpret_cast<CTCBaseInterfaceMap*>(&overlay->interfaceMap)->LowerBound(&local_8));
        if ((piVar3 == overlay->interfaceMap.end) || (static_cast<std::int32_t>(0x12) < static_cast<std::int32_t>(piVar3->key))) {
            piVar3 = overlay->interfaceMap.end;
        }
        CTCBase* iVar1 = piVar3->value;
        *outInterface = iVar1;
        uVar2 = (static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(iVar1)) & 0xFFFFFF00u) | 1u;
    }
    return uVar2;
}