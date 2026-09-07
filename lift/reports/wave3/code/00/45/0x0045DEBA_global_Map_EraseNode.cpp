#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct MapEraseNodeOverlay {
    std::int32_t allocatorBase; // +0x00
    std::int32_t count;         // +0x04
};

static_assert(offsetof(MapEraseNodeOverlay, allocatorBase) == 0x00);
static_assert(offsetof(MapEraseNodeOverlay, count) == 0x04);

extern "C" void* CMemoryAllocatorVariableSize__Erase(
    std::uint32_t param_2,
    std::uint32_t a2,
    std::uint32_t a3,
    std::uint32_t a4
);

void __thiscall Map_EraseNode(MapEraseNodeOverlay* self, std::uint32_t param_2)
{
    const std::uint32_t iVar1 = static_cast<std::uint32_t>(self->allocatorBase);
    void* const memory = CMemoryAllocatorVariableSize__Erase(param_2, iVar1 + 4, iVar1 + 8, iVar1 + 0x0C);
    if (memory != nullptr) {
        std::free(memory);
    }
    self->count = self->count - 1;
}