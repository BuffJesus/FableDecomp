#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct BoundingBoxNodeOverlay
{
    std::byte pad_0[0x08];
    std::uint32_t value_08;
    std::uint32_t value_0C;
};

static_assert(offsetof(BoundingBoxNodeOverlay, value_08) == 0x08);
static_assert(offsetof(BoundingBoxNodeOverlay, value_0C) == 0x0C);
static_assert(sizeof(BoundingBoxNodeOverlay) == 0x10);

void __stdcall CEngineSceneGrid_AllocBoundingBoxNode(std::uint32_t* param_1)
{
    void* const allocation = std::malloc(0x10);

    if ((reinterpret_cast<std::uintptr_t>(allocation) + 0x08) != 0) {
        auto* const node = static_cast<BoundingBoxNodeOverlay*>(allocation);
        node->value_08 = param_1[0];
        node->value_0C = param_1[1];
    }
}