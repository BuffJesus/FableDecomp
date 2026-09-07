#include <cstddef>
#include <cstdint>

struct TreeNodeOverlay
{
    std::uint8_t value;       // 0x00
    std::byte pad01[3];       // 0x01
    void* link04;             // 0x04
    TreeNodeOverlay* next08;  // 0x08
    TreeNodeOverlay* child0C; // 0x0C
    std::byte payload[1];     // 0x10
};

static_assert(offsetof(TreeNodeOverlay, value) == 0x00);
static_assert(offsetof(TreeNodeOverlay, link04) == 0x04);
static_assert(offsetof(TreeNodeOverlay, next08) == 0x08);
static_assert(offsetof(TreeNodeOverlay, child0C) == 0x0C);
static_assert(offsetof(TreeNodeOverlay, payload) == 0x10);

extern "C" void* __stdcall Allocator_Alloc_Pair_With_CCharString(const void* sourcePayload);

extern "C" TreeNodeOverlay* __stdcall Tree_Clone_Recursive(
    TreeNodeOverlay* source,
    void* linkValue)
{
    TreeNodeOverlay* const clone = static_cast<TreeNodeOverlay*>(
        Allocator_Alloc_Pair_With_CCharString(source->payload));
    const std::uint8_t sourceValue = source->value;

    clone->next08 = nullptr;
    clone->child0C = nullptr;
    clone->value = sourceValue;
    clone->link04 = linkValue;

    if (source->child0C != nullptr)
    {
        clone->child0C = Tree_Clone_Recursive(source->child0C, clone);
    }

    linkValue = clone;
    for (TreeNodeOverlay* sibling = source->next08; sibling != nullptr; sibling = sibling->next08)
    {
        TreeNodeOverlay* const siblingClone = static_cast<TreeNodeOverlay*>(
            Allocator_Alloc_Pair_With_CCharString(sibling->payload));
        const std::uint8_t siblingValue = sibling->value;

        siblingClone->next08 = nullptr;
        siblingClone->child0C = nullptr;
        siblingClone->value = siblingValue;
        static_cast<TreeNodeOverlay*>(linkValue)->next08 = siblingClone;
        siblingClone->link04 = linkValue;

        if (sibling->child0C != nullptr)
        {
            siblingClone->child0C = Tree_Clone_Recursive(sibling->child0C, siblingClone);
        }

        linkValue = siblingClone;
    }

    return clone;
}