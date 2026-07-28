#include <bitset>
#include <cstddef>
#include <cstdint>

using ETCInterfaceType = std::int32_t;

struct CTCBase;

template <typename K, typename V>
struct CKeyPairCompareLess;

template <typename K, typename V, typename Compare>
struct CVectorMap {
    std::int32_t* field_00;
    std::int32_t* field_04;

    std::int32_t* LowerBound(const K* key);
};

struct CTC_FindInterface_0x10e_Overlay {
    std::byte pad_0000[0x20];
    std::bitset<0x10F> field_020;
    CVectorMap<
        ETCInterfaceType,
        CTCBase*,
        CKeyPairCompareLess<ETCInterfaceType, CTCBase*>
    > field_044;
};

static_assert(offsetof(CTC_FindInterface_0x10e_Overlay, field_020) == 0x20);
static_assert(offsetof(CTC_FindInterface_0x10e_Overlay, field_044) == 0x44);
static_assert(
    offsetof(
        CVectorMap<
            ETCInterfaceType,
            CTCBase*,
            CKeyPairCompareLess<ETCInterfaceType, CTCBase*>
        >,
        field_04
    ) == 0x04
);

extern "C" std::uint32_t __thiscall std__bitset__test(
    const std::bitset<0x10F>* self,
    std::size_t index
);

std::uint32_t __thiscall CTC_FindInterface_0x10e(
    CTC_FindInterface_0x10e_Overlay* self,
    CTCBase** outInterface)
{
    ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(self);
    std::uint32_t testResult = std__bitset__test(&self->field_020, 0x10E);

    if ((static_cast<char>(testResult)) == '\0') {
        return testResult & 0xFFFFFF00u;
    }

    interfaceType = 0x10E;
    std::int32_t* entry = self->field_044.LowerBound(&interfaceType);
    if ((entry == self->field_044.field_04) || (0x10E < *entry)) {
        entry = self->field_044.field_04;
    }

    const std::uint32_t storedInterfaceValue = static_cast<std::uint32_t>(entry[1]);
    *outInterface = reinterpret_cast<CTCBase*>(storedInterfaceValue);
    return (storedInterfaceValue & 0xFFFFFF00u) | 0x01u;
}