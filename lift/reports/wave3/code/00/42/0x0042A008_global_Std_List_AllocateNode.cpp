#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct StdListNodeValueOverlay {
    std::byte pad_0[0x8];
    std::uint32_t value;
};

static_assert(offsetof(StdListNodeValueOverlay, value) == 0x8);
static_assert(sizeof(StdListNodeValueOverlay) == 0xC);

void __stdcall Std_List_AllocateNode(std::uint32_t* param_1)
{
    void* const pvVar1 = std::malloc(0xCu);
    auto* const valueSlot =
        reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(pvVar1) + 0x8);

    if (valueSlot != nullptr) {
        *valueSlot = *param_1;
    }
}