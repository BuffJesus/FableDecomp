#include <cstddef>

struct CTCCarryableTreeNode {
    std::byte pad_00[0x08];
    CTCCarryableTreeNode* link_08;
    CTCCarryableTreeNode* link_0C;
};
static_assert(offsetof(CTCCarryableTreeNode, link_08) == 0x08);
static_assert(offsetof(CTCCarryableTreeNode, link_0C) == 0x0C);

struct CTCCarryableTreeState {
    void* field_00;
    CTCCarryableTreeNode* root;
    CTCCarryableTreeNode* field_08;
    CTCCarryableTreeNode* field_0C;
};
static_assert(offsetof(CTCCarryableTreeState, root) == 0x04);
static_assert(offsetof(CTCCarryableTreeState, field_08) == 0x08);
static_assert(offsetof(CTCCarryableTreeState, field_0C) == 0x0C);

struct CTCCarryable {
    CTCCarryableTreeState* treeState;
    int field_04;

    void OnKill();
};
static_assert(offsetof(CTCCarryable, treeState) == 0x00);
static_assert(offsetof(CTCCarryable, field_04) == 0x04);

extern CTCCarryableTreeNode* CTCCarryable_Clone_Tree(CTCCarryableTreeNode* node, CTCCarryableTreeState* owner);

CTCCarryable* __thiscall CTCCarryable_CopyAssign(CTCCarryable* self, CTCCarryable* other) {
    if (self != other) {
        self->OnKill();
        self->field_04 = 0;

        if (other->treeState->root == nullptr) {
            self->treeState->root = nullptr;
            self->treeState->field_08 = reinterpret_cast<CTCCarryableTreeNode*>(self->treeState);
            self->treeState->field_0C = reinterpret_cast<CTCCarryableTreeNode*>(self->treeState);
        } else {
            CTCCarryableTreeState* const owner = self->treeState;
            owner->root = CTCCarryable_Clone_Tree(other->treeState->root, owner);

            CTCCarryableTreeNode* node = self->treeState->root;
            CTCCarryableTreeNode* last;
            do {
                last = node;
                node = last->link_08;
            } while (node != nullptr);
            self->treeState->field_08 = last;

            node = self->treeState->root;
            do {
                last = node;
                node = last->link_0C;
            } while (node != nullptr);
            self->treeState->field_0C = last;

            self->field_04 = other->field_04;
        }
    }

    return self;
}