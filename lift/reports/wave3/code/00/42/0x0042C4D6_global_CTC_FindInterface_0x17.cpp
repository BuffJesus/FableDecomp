using InterfaceMap_0x17 = CVectorMap<
    ETCInterfaceType,
    CTCBase*,
    CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CTCInterfaceEntryOverlay_0x17 {
    ETCInterfaceType key;
    CTCBase* value;
};
static_assert(offsetof(CTCInterfaceEntryOverlay_0x17, key) == 0x0);
static_assert(offsetof(CTCInterfaceEntryOverlay_0x17, value) == 0x4);

static_assert(sizeof(std::bitset<0x120>) == 0x24);

struct CTC_FindInterface_0x17_Overlay {
    std::byte pad_00[0x20];
    std::bitset<0x120> interface_type_bits; // this + 0x20
    std::byte interface_map_storage[0x04];  // this + 0x44
    CTCInterfaceEntryOverlay_0x17* interface_map_end; // this + 0x48
};
static_assert(offsetof(CTC_FindInterface_0x17_Overlay, interface_type_bits) == 0x20);
static_assert(offsetof(CTC_FindInterface_0x17_Overlay, interface_map_storage) == 0x44);
static_assert(offsetof(CTC_FindInterface_0x17_Overlay, interface_map_end) == 0x48);

std::uint32_t __thiscall CTC_FindInterface_0x17(void* this_ptr, CTCBase** out_interface)
{
    auto* const self = reinterpret_cast<CTC_FindInterface_0x17_Overlay*>(this_ptr);

    std::uint32_t result =
        static_cast<std::uint32_t>(self->interface_type_bits.test(0x17));

    if (static_cast<std::uint8_t>(result) == 0) {
        result &= 0xFFFFFF00u;
    } else {
        ETCInterfaceType search_key = static_cast<ETCInterfaceType>(0x17);

        auto* entry = reinterpret_cast<CTCInterfaceEntryOverlay_0x17*>(
            InterfaceMap_0x17::LowerBound(
                reinterpret_cast<InterfaceMap_0x17*>(&self->interface_map_storage),
                &search_key));

        if ((entry == self->interface_map_end) ||
            (static_cast<ETCInterfaceType>(0x17) < entry->key)) {
            entry = self->interface_map_end;
        }

        CTCBase* const found_interface = entry->value;
        *out_interface = found_interface;

        result =
            (static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(found_interface)) &
             0xFFFFFF00u) |
            1u;
    }

    return result;
}