#include <cstddef>
#include <cstdint>

struct PointerSlot {
    void* value;
};

static_assert(offsetof(PointerSlot, value) == 0);

extern "C" std::uint32_t __stdcall Getter_TripleDeref(PointerSlot* param_1)
{
    const auto level1 = static_cast<PointerSlot*>(param_1->value);
    const auto level2 = static_cast<PointerSlot*>(level1->value);
    const auto level3 = static_cast<std::uint32_t*>(level2->value);
    return *level3;
}