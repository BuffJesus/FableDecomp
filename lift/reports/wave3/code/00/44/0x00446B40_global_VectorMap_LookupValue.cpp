#include <cstddef>
#include <cstdint>

struct VectorMapEntryOverlay {
    std::int32_t key;   // +0x00
    void* valuePtr;     // +0x04
};
static_assert(offsetof(VectorMapEntryOverlay, key) == 0x00);
static_assert(offsetof(VectorMapEntryOverlay, valuePtr) == 0x04);

struct VectorMapOverlay {
    std::byte unknown_00[0x04];
    VectorMapEntryOverlay* end; // +0x04

    VectorMapEntryOverlay* __thiscall LowerBound(const std::int32_t* key);
};
static_assert(offsetof(VectorMapOverlay, end) == 0x04);

struct VectorMapLookupValueOwner {
    std::byte unknown_0000[0x7A8];
    VectorMapOverlay map; // +0x7A8
};
static_assert(offsetof(VectorMapLookupValueOwner, map) == 0x7A8);
static_assert(offsetof(VectorMapLookupValueOwner, map) + offsetof(VectorMapOverlay, end) == 0x7AC);

std::uint32_t __thiscall VectorMap_LookupValue(
    VectorMapLookupValueOwner* self,
    std::uint32_t param_2,
    std::int32_t key)
{
    VectorMapEntryOverlay* it = self->map.LowerBound(&key);

    if ((it == self->map.end) || (key < it->key)) {
        it = self->map.end;
    }

    if (it != self->map.end) {
        using VtableSlot18Fn = std::uint32_t(__thiscall*)(void*, std::uint32_t);

        auto* const object = it->valuePtr;
        auto* const vtableBytes = *reinterpret_cast<std::uint8_t**>(object);
        const auto fn = *reinterpret_cast<VtableSlot18Fn*>(vtableBytes + 0x18);

        return fn(object, param_2);
    }

    return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(it)) & 0xFFFFFF00u;
}