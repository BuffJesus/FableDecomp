#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

struct PairWithCCharStringOverlay {
    std::byte pad_00[0x10];
    std::uint32_t first;
    CCharString second;
};
static_assert(offsetof(PairWithCCharStringOverlay, first) == 0x10);
static_assert(offsetof(PairWithCCharStringOverlay, second) == 0x14);
static_assert(sizeof(PairWithCCharStringOverlay) == 0x18);

struct PairWithCCharStringSourceOverlay {
    std::uint32_t first;
    CCharString second;
};
static_assert(offsetof(PairWithCCharStringSourceOverlay, first) == 0x0);
static_assert(offsetof(PairWithCCharStringSourceOverlay, second) == 0x4);

void* __cdecl Allocator_Alloc_Pair_With_CCharString(const PairWithCCharStringSourceOverlay* param_1)
{
    void* pvVar1 = std::malloc(0x18);

    auto* const first_ptr = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uintptr_t>(pvVar1) + 0x10);

    if (first_ptr != nullptr) {
        *first_ptr = param_1->first;

        ::new (reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(pvVar1) + 0x14))
            CCharString(param_1->second);
    }

    return pvVar1;
}