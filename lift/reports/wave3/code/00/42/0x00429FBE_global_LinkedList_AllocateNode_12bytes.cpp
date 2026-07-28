#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct LinkedListNode12BytesOverlay {
    std::byte pad_0[8];
    std::uint32_t value_8;
};

static_assert(offsetof(LinkedListNode12BytesOverlay, value_8) == 8);

void LinkedList_AllocateNode_12bytes(std::uint32_t* param_1)
{
    void* pvVar1 = std::malloc(0x0C);

    auto* value_8 = reinterpret_cast<decltype(LinkedListNode12BytesOverlay::value_8)*>(
        static_cast<int>(reinterpret_cast<std::intptr_t>(pvVar1)) +
        static_cast<int>(offsetof(LinkedListNode12BytesOverlay, value_8))
    );

    if (value_8 != nullptr) {
        *value_8 = *param_1;
    }
}