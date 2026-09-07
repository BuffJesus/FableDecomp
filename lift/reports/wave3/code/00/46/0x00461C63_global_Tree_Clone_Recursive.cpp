#include <cstddef>
#include <cstdint>

struct TreeNodeOverlay {
    std::uint8_t value;        // 0x00
    std::uint8_t pad01[3];     // 0x01
    TreeNodeOverlay* link04;   // 0x04
    TreeNodeOverlay* next08;   // 0x08
    TreeNodeOverlay* child0C;  // 0x0C
};

static_assert(offsetof(TreeNodeOverlay, value) == 0x00);
static_assert(offsetof(TreeNodeOverlay, link04) == 0x04);
static_assert(offsetof(TreeNodeOverlay, next08) == 0x08);
static_assert(offsetof(TreeNodeOverlay, child0C) == 0x0C);
static_assert(sizeof(TreeNodeOverlay) == 0x10);

extern "C" void* __stdcall Allocator_Alloc_Struct_Smaller(const void* size_from);

TreeNodeOverlay* __stdcall Tree_Clone_Recursive(TreeNodeOverlay* source, TreeNodeOverlay* parent)
{
    auto* const clone = static_cast<TreeNodeOverlay*>(
        Allocator_Alloc_Struct_Smaller(reinterpret_cast<const std::uint8_t*>(source) + 0x10));

    const std::uint8_t sourceValue = source->value;
    clone->next08 = nullptr;
    clone->child0C = nullptr;
    clone->value = sourceValue;
    clone->link04 = parent;

    if (source->child0C != nullptr) {
        clone->child0C = Tree_Clone_Recursive(source->child0C, clone);
    }

    parent = clone;
    for (TreeNodeOverlay* sibling = source->next08; sibling != nullptr; sibling = sibling->next08) {
        auto* const siblingClone = static_cast<TreeNodeOverlay*>(
            Allocator_Alloc_Struct_Smaller(reinterpret_cast<const std::uint8_t*>(sibling) + 0x10));

        const std::uint8_t siblingValue = sibling->value;
        siblingClone->next08 = nullptr;
        siblingClone->child0C = nullptr;
        siblingClone->value = siblingValue;
        parent->next08 = siblingClone;
        siblingClone->link04 = parent;

        if (sibling->child0C != nullptr) {
            siblingClone->child0C = Tree_Clone_Recursive(sibling->child0C, siblingClone);
        }

        parent = siblingClone;
    }

    return clone;
}