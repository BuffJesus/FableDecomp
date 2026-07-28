#include <cstddef>
#include <cstdint>

struct CTCBase;
enum ETCInterfaceType : int;

template <typename K, std::size_t N>
class bitset
{
public:
    std::uint32_t test(K value) const;
};

template <typename K, typename V>
struct CKeyPairCompareLess;

template <typename K, typename V, typename Compare>
class CVectorMap
{
public:
    int* LowerBound(const K* key);
};

struct CTCBase_FindInterfaceType_0xd0_MapOverlay
{
    int* beginPair;
    int* endPair;
};

static_assert(offsetof(CTCBase_FindInterfaceType_0xd0_MapOverlay, beginPair) == 0x0);
static_assert(offsetof(CTCBase_FindInterfaceType_0xd0_MapOverlay, endPair) == 0x4);

struct CTCBase_FindInterfaceType_0xd0_Overlay
{
    std::byte pad00[0x20];
    bitset<ETCInterfaceType, 0xD1> interfaceTypeBits;
    std::byte pad20[0x44 - 0x20 - sizeof(interfaceTypeBits)];
    CTCBase_FindInterfaceType_0xd0_MapOverlay interfaceMapRaw;
};

static_assert(offsetof(CTCBase_FindInterfaceType_0xd0_Overlay, interfaceTypeBits) == 0x20);
static_assert(offsetof(CTCBase_FindInterfaceType_0xd0_Overlay, interfaceMapRaw) == 0x44);
static_assert(
    offsetof(CTCBase_FindInterfaceType_0xd0_Overlay, interfaceMapRaw) +
        offsetof(CTCBase_FindInterfaceType_0xd0_MapOverlay, endPair) ==
    0x48);

std::uint32_t __thiscall CTCBase_FindInterfaceType_0xd0(CTCBase* self, CTCBase** outInterface)
{
    auto* const overlay = reinterpret_cast<CTCBase_FindInterfaceType_0xd0_Overlay*>(self);
    ETCInterfaceType local_8 = static_cast<ETCInterfaceType>(reinterpret_cast<std::uintptr_t>(self));

    std::uint32_t result = overlay->interfaceTypeBits.test(static_cast<ETCInterfaceType>(0xD0));
    if (static_cast<char>(result) == '\0')
    {
        return result & 0xFFFFFF00u;
    }

    local_8 = static_cast<ETCInterfaceType>(0xD0);

    auto* const interfaceMap =
        reinterpret_cast<CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>*>(
            &overlay->interfaceMapRaw);

    int* selectedPair = interfaceMap->LowerBound(&local_8);
    if (selectedPair == overlay->interfaceMapRaw.endPair || 0xD0 < selectedPair[0])
    {
        selectedPair = overlay->interfaceMapRaw.endPair;
    }

    const int loadedValue = selectedPair[1];
    *reinterpret_cast<int*>(outInterface) = loadedValue;
    return (static_cast<std::uint32_t>(loadedValue) & 0xFFFFFF00u) | 1u;
}