#include <cstddef>
#include <cstdint>

struct BinarySearchTreeNodeOverlay {
    std::uint32_t pad_0x00;
    BinarySearchTreeNodeOverlay* link_0x04;
    BinarySearchTreeNodeOverlay* left_0x08;
    BinarySearchTreeNodeOverlay* right_0x0C;
    std::uint32_t key_0x10;
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, link_0x04) == 0x04);
static_assert(offsetof(BinarySearchTreeNodeOverlay, left_0x08) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right_0x0C) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key_0x10) == 0x10);

struct BinarySearchTreeOverlay {
    BinarySearchTreeNodeOverlay* head_0x00;
};

static_assert(offsetof(BinarySearchTreeOverlay, head_0x00) == 0x00);

BinarySearchTreeNodeOverlay* __thiscall BinarySearchTree_FindLessOrEqual_Copy(
    BinarySearchTreeOverlay* self,
    const std::uint32_t* key)
{
    BinarySearchTreeNodeOverlay* const head = self->head_0x00;
    BinarySearchTreeNodeOverlay* result = head;

    if (head->link_0x04 != nullptr) {
        BinarySearchTreeNodeOverlay* current = head->link_0x04;
        do {
            BinarySearchTreeNodeOverlay* next;
            if (current->key_0x10 < *key) {
                next = current->right_0x0C;
            } else {
                next = current->left_0x08;
                result = current;
            }
            current = next;
        } while (current != nullptr);
    }

    if ((result == head) || (*key < result->key_0x10)) {
        result = head;
    }

    return result;
}