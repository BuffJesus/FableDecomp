struct CTCBase_FindInterfaceType_0x19_MapEntry
{
    ETCInterfaceType key;
    CTCBase* value;
};

static_assert(offsetof(CTCBase_FindInterfaceType_0x19_MapEntry, key) == 0x0);
static_assert(offsetof(CTCBase_FindInterfaceType_0x19_MapEntry, value) == 0x4);

struct CTCBase_FindInterfaceType_0x19_Overlay
{
    std::byte pad_00[0x20];
    std::byte interfaceTypeBitsStorage[0x24];
    std::byte interfaceMapStorage[0x8];
};

static_assert(offsetof(CTCBase_FindInterfaceType_0x19_Overlay, interfaceTypeBitsStorage) == 0x20);
static_assert(offsetof(CTCBase_FindInterfaceType_0x19_Overlay, interfaceMapStorage) == 0x44);

uint32_t __thiscall CTCBase_FindInterfaceType_0x19(CTCBase* self, CTCBase** outInterface)
{
    auto* const overlay = reinterpret_cast<CTCBase_FindInterfaceType_0x19_Overlay*>(self);

    ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(self);
    uint32_t result = static_cast<uint32_t>(
        reinterpret_cast<const std::bitset<0x20>*>(overlay->interfaceTypeBitsStorage)->test(0x19));

    if (static_cast<char>(result) == '\0')
    {
        result &= 0xFFFFFF00u;
    }
    else
    {
        searchKey = static_cast<ETCInterfaceType>(0x19);

        auto* entry = reinterpret_cast<CTCBase_FindInterfaceType_0x19_MapEntry*>(
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>::LowerBound(
                reinterpret_cast<CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>*>(overlay->interfaceMapStorage),
                &searchKey));

        auto* const end = *reinterpret_cast<CTCBase_FindInterfaceType_0x19_MapEntry**>(
            reinterpret_cast<std::byte*>(self) + 0x48);

        if ((entry == end) || (static_cast<ETCInterfaceType>(0x19) < entry->key))
        {
            entry = end;
        }

        CTCBase* const value = entry->value;
        *outInterface = value;
        result = (static_cast<uint32_t>(reinterpret_cast<uintptr_t>(value)) & 0xFFFFFF00u) | 1u;
    }

    return result;
}