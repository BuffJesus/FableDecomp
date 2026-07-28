#include <cstddef>
#include <cstdint>

struct CTC_InterfaceBits_20 {
    std::uint32_t storage[9];
};
static_assert(sizeof(CTC_InterfaceBits_20) == 0x24);

struct CTC_MapAt44 {
    std::int32_t* data_at_00;
    std::int32_t* end_at_04;
};
static_assert(sizeof(CTC_MapAt44) == 0x08);
static_assert(offsetof(CTC_MapAt44, end_at_04) == 0x04);

struct CTC_FindInterface_0x11_Overlay {
    std::byte pad_00[0x20];
    CTC_InterfaceBits_20 interface_bits_at_20;
    CTC_MapAt44 map_at_44;
};

static_assert(offsetof(CTC_FindInterface_0x11_Overlay, interface_bits_at_20) == 0x20);
static_assert(offsetof(CTC_FindInterface_0x11_Overlay, map_at_44) == 0x44);
static_assert(offsetof(CTC_FindInterface_0x11_Overlay, map_at_44) + offsetof(CTC_MapAt44, end_at_04) == 0x48);

extern std::uint32_t __thiscall std__bitset__test(CTC_InterfaceBits_20* self, std::uint32_t bit_index);
extern std::int32_t* __thiscall CVectorMap_LowerBound(CTC_MapAt44* self, const std::int32_t* key);

std::uint32_t __thiscall CTC_FindInterface_0x11(CTC_FindInterface_0x11_Overlay* self, std::int32_t* out_interface)
{
    std::uint32_t test_result = std__bitset__test(&self->interface_bits_at_20, 0x11);
    if (static_cast<std::uint8_t>(test_result) == 0) {
        return test_result & 0xFFFFFF00u;
    }

    std::int32_t key = 0x11;
    std::int32_t* entry = CVectorMap_LowerBound(&self->map_at_44, &key);
    if (entry == self->map_at_44.end_at_04 || 0x11 < *entry) {
        entry = self->map_at_44.end_at_04;
    }

    const std::int32_t value = entry[1];
    *out_interface = value;
    return (static_cast<std::uint32_t>(value) & 0xFFFFFF00u) | 1u;
}