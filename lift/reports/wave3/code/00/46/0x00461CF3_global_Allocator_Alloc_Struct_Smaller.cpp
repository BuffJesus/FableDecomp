#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct AllocatorAllocStructSmallerOverlay {
    std::byte padding[0x10];
    std::uint32_t value;
};

static_assert(offsetof(AllocatorAllocStructSmallerOverlay, value) == 0x10);
static_assert(sizeof(AllocatorAllocStructSmallerOverlay) == 0x14);

void __cdecl Allocator_Alloc_Struct_Smaller(std::uint32_t* param_1)
{
    void* const pvVar1 = std::malloc(0x14);
    const std::uint32_t value_address =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(pvVar1)) +
        static_cast<std::uint32_t>(offsetof(AllocatorAllocStructSmallerOverlay, value));
    auto* const value_ptr =
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(value_address));

    if (value_ptr != nullptr) {
        *value_ptr = *param_1;
    }
}