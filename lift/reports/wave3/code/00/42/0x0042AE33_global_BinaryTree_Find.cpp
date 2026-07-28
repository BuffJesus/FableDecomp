#include <cstddef>
#include <cstdint>

struct BinaryTreeEntryOverlay {
    BinaryTreeEntryOverlay* field_0x00;
    BinaryTreeEntryOverlay* field_0x04;
    BinaryTreeEntryOverlay* child_at_0x08;
    BinaryTreeEntryOverlay* child_at_0x0C;
    std::int32_t key_at_0x10;
};

static_assert(offsetof(BinaryTreeEntryOverlay, field_0x00) == 0x00);
static_assert(offsetof(BinaryTreeEntryOverlay, field_0x04) == 0x04);
static_assert(offsetof(BinaryTreeEntryOverlay, child_at_0x08) == 0x08);
static_assert(offsetof(BinaryTreeEntryOverlay, child_at_0x0C) == 0x0C);
static_assert(offsetof(BinaryTreeEntryOverlay, key_at_0x10) == 0x10);

struct BinaryTreeOuterOverlay {
    BinaryTreeEntryOverlay* field_0x00;
};

static_assert(offsetof(BinaryTreeOuterOverlay, field_0x00) == 0x00);

BinaryTreeEntryOverlay* __thiscall BinaryTree_Find(
    BinaryTreeOuterOverlay* self,
    const std::int32_t* key)
{
    BinaryTreeEntryOverlay* tree = self->field_0x00;
    BinaryTreeEntryOverlay* result = tree;

    if (tree->field_0x04 != nullptr) {
        BinaryTreeEntryOverlay* current = tree->field_0x04;
        do {
            BinaryTreeEntryOverlay* next;
            if (current->key_at_0x10 < *key) {
                next = current->child_at_0x0C;
            } else {
                next = current->child_at_0x08;
                result = current;
            }
            current = next;
        } while (next != nullptr);
    }

    return result;
}