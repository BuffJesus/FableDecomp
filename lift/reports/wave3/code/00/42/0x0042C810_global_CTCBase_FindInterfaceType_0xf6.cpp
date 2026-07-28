using ETCInterfaceType = unsigned int;

struct CTCBase_FindInterfaceType_0xf6_MapOverlay
{
    int* begin; // +0x00
    int* end;   // +0x04
};

static_assert(offsetof(CTCBase_FindInterfaceType_0xf6_MapOverlay, begin) == 0x0);
static_assert(offsetof(CTCBase_FindInterfaceType_0xf6_MapOverlay, end) == 0x4);

struct CTCBase_FindInterfaceType_0xf6_Overlay
{
    unsigned char pad_00[0x20];
    unsigned char interface_type_bitset_20[0x24];
    CTCBase_FindInterfaceType_0xf6_MapOverlay interface_map_44;
};

static_assert(offsetof(CTCBase_FindInterfaceType_0xf6_Overlay, interface_type_bitset_20) == 0x20);
static_assert(offsetof(CTCBase_FindInterfaceType_0xf6_Overlay, interface_map_44) == 0x44);

extern "C" unsigned char __thiscall std__bitset__test(void* self, unsigned int index);

extern "C" int* __thiscall
CVectorMap_ETCInterfaceType_CTCBase_ptr_CKeyPairCompareLess_ETCInterfaceType_CTCBase_ptr_LowerBound(
    CTCBase_FindInterfaceType_0xf6_MapOverlay* self,
    const ETCInterfaceType* key);

unsigned int __thiscall ::CTCBase_FindInterfaceType_0xf6(void* self, int* out_value)
{
    auto* const obj = static_cast<CTCBase_FindInterfaceType_0xf6_Overlay*>(self);

    ETCInterfaceType key = static_cast<ETCInterfaceType>(obj);
    unsigned int result = std__bitset__test(&obj->interface_type_bitset_20, 0xF6);

    if (static_cast<char>(result) == '\0')
    {
        result &= 0xFFFFFF00u;
    }
    else
    {
        key = 0xF6;

        int* entry =
            CVectorMap_ETCInterfaceType_CTCBase_ptr_CKeyPairCompareLess_ETCInterfaceType_CTCBase_ptr_LowerBound(
                &obj->interface_map_44,
                &key);

        if ((entry == obj->interface_map_44.end) || (0xF6 < static_cast<unsigned int>(*entry)))
        {
            entry = obj->interface_map_44.end;
        }

        const int value = entry[1];
        *out_value = value;
        result = (static_cast<unsigned int>(value) & 0xFFFFFF00u) | 1u;
    }

    return result;
}