#include <bitset>
#include <cstddef>
#include <cstdint>

enum ETCInterfaceType : std::int32_t;
struct CTCBase;

struct CTCBase_FindInterfaceType_0x16_Entry {
    ETCInterfaceType key; // 0x00
    int value;            // 0x04
};
static_assert(offsetof(CTCBase_FindInterfaceType_0x16_Entry, key) == 0x00);
static_assert(offsetof(CTCBase_FindInterfaceType_0x16_Entry, value) == 0x04);
static_assert(sizeof(CTCBase_FindInterfaceType_0x16_Entry) == 0x08);

struct CTCBase_FindInterfaceType_0x16_MapOverlay {
    CTCBase_FindInterfaceType_0x16_Entry* field_0; // 0x00
    CTCBase_FindInterfaceType_0x16_Entry* field_4; // 0x04

    CTCBase_FindInterfaceType_0x16_Entry* LowerBound(const ETCInterfaceType* key);
};
static_assert(offsetof(CTCBase_FindInterfaceType_0x16_MapOverlay, field_0) == 0x00);
static_assert(offsetof(CTCBase_FindInterfaceType_0x16_MapOverlay, field_4) == 0x04);
static_assert(sizeof(CTCBase_FindInterfaceType_0x16_MapOverlay) == 0x08);

struct CTCBase_FindInterfaceType_0x16_Overlay {
    std::byte pad_00[0x20];
    std::bitset<0x120> interfaceBits;                    // 0x20
    CTCBase_FindInterfaceType_0x16_MapOverlay field_44; // 0x44
};
static_assert(offsetof(CTCBase_FindInterfaceType_0x16_Overlay, interfaceBits) == 0x20);
static_assert(sizeof(std::bitset<0x120>) == 0x24);
static_assert(offsetof(CTCBase_FindInterfaceType_0x16_Overlay, field_44) == 0x44);

std::uint32_t __thiscall CTCBase_FindInterfaceType_0x16(CTCBase* self, int* outValue)
{
    auto* const overlay = reinterpret_cast<CTCBase_FindInterfaceType_0x16_Overlay*>(self);

    ETCInterfaceType local_8 = static_cast<ETCInterfaceType>(reinterpret_cast<std::uintptr_t>(self));
    std::uint32_t result = static_cast<std::uint32_t>(overlay->interfaceBits.test(0x16));

    if (static_cast<char>(result) == '\0') {
        result &= 0xFFFFFF00u;
    } else {
        local_8 = static_cast<ETCInterfaceType>(0x16);

        CTCBase_FindInterfaceType_0x16_Entry* entry = overlay->field_44.LowerBound(&local_8);
        if (entry == overlay->field_44.field_4 || static_cast<std::int32_t>(0x16) < static_cast<std::int32_t>(entry->key)) {
            entry = overlay->field_44.field_4;
        }

        const int value = entry->value;
        *outValue = value;
        result = (static_cast<std::uint32_t>(value) & 0xFFFFFF00u) | 1u;
    }

    return result;
}