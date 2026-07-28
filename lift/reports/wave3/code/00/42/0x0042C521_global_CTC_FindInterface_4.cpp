std::uint32_t __thiscall CTC_FindInterface_4(void* this_ptr, int* outValue)
{
    using MapType = CVectorMap<
        ETCInterfaceType,
        CTCBase*,
        CKeyPairCompareLess<ETCInterfaceType, CTCBase*>
    >;

    struct CTC_FindInterface_4_Overlay
    {
        std::byte pad_0000[0x20];
        decltype(((std::bitset<32>*)nullptr))::element_type interfaceBits;
        std::byte pad_0024[0x20];
        std::byte mapStorage[0x4];
        int* mapEnd;
    };

    static_assert(offsetof(CTC_FindInterface_4_Overlay, interfaceBits) == 0x20);
    static_assert(offsetof(CTC_FindInterface_4_Overlay, mapStorage) == 0x44);
    static_assert(offsetof(CTC_FindInterface_4_Overlay, mapEnd) == 0x48);

    auto* const self = static_cast<CTC_FindInterface_4_Overlay*>(this_ptr);

    ETCInterfaceType lookupKey{};
    std::uint32_t result = self->interfaceBits.test(4);

    if (static_cast<char>(result) == '\0')
    {
        result &= 0xFFFFFF00u;
    }
    else
    {
        lookupKey = static_cast<ETCInterfaceType>(4);

        int* it = reinterpret_cast<int*>(
            MapType::LowerBound(
                reinterpret_cast<MapType*>(reinterpret_cast<std::byte*>(self) + 0x44),
                &lookupKey
            )
        );

        if ((it == self->mapEnd) || (4 < *it))
        {
            it = self->mapEnd;
        }

        const int value = it[1];
        *outValue = value;
        result = (static_cast<std::uint32_t>(value) & 0xFFFFFF00u) | 1u;
    }

    return result;
}