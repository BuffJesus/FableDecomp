#include <cstddef>
#include <cstdint>

struct RBTreeObjectOverlay {
    std::int32_t sentinel;  // 0x00
    std::int32_t nodeCount; // 0x04
    std::int32_t leftmost;  // 0x08
    std::int32_t rightmost; // 0x0C

    std::int32_t __thiscall TreeNode_AllocData(std::int32_t* value);
};

static_assert(offsetof(RBTreeObjectOverlay, sentinel) == 0x00);
static_assert(offsetof(RBTreeObjectOverlay, nodeCount) == 0x04);
static_assert(offsetof(RBTreeObjectOverlay, leftmost) == 0x08);
static_assert(offsetof(RBTreeObjectOverlay, rightmost) == 0x0C);

struct RBTreeHeaderNodeOverlay {
    std::uint8_t pad_00[0x04];
    std::int32_t root;      // 0x04
    std::int32_t leftmost;  // 0x08
    std::int32_t rightmost; // 0x0C
};

static_assert(offsetof(RBTreeHeaderNodeOverlay, root) == 0x04);
static_assert(offsetof(RBTreeHeaderNodeOverlay, leftmost) == 0x08);
static_assert(offsetof(RBTreeHeaderNodeOverlay, rightmost) == 0x0C);

struct RBTreeNodeOverlay {
    std::uint8_t pad_00[0x04];
    std::int32_t parent; // 0x04
    std::int32_t left;   // 0x08
    std::int32_t right;  // 0x0C
    std::int32_t key;    // 0x10
};

static_assert(offsetof(RBTreeNodeOverlay, parent) == 0x04);
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, key) == 0x10);

void CMemoryAllocatorVariableSize__BalanceTree(std::int32_t node, std::int32_t headerPlus4);

void __thiscall ::RBTree_Insert_Internal(
    std::int32_t* param_1,
    std::int32_t* param_2,
    std::int32_t param_3,
    std::int32_t param_4,
    std::int32_t* param_5,
    std::int32_t param_6)
{
    auto* const tree = reinterpret_cast<RBTreeObjectOverlay*>(param_1);

    std::int32_t iVar1;
    std::int32_t iVar2;

    if ((param_4 == tree->sentinel) ||
        ((param_6 == 0) && ((param_3 != 0) || (*param_5 < reinterpret_cast<RBTreeNodeOverlay*>(param_4)->key)))) {
        iVar1 = tree->TreeNode_AllocData(param_5);
        reinterpret_cast<RBTreeNodeOverlay*>(param_4)->left = iVar1;
        iVar2 = tree->sentinel;
        if (param_4 != iVar2) {
            auto* const header = reinterpret_cast<RBTreeHeaderNodeOverlay*>(iVar2);
            if (param_4 == header->leftmost) {
                header->leftmost = iVar1;
            }
            goto LAB_00461AA0;
        }
        reinterpret_cast<RBTreeHeaderNodeOverlay*>(iVar2)->root = iVar1;
        iVar2 = tree->sentinel;
    } else {
        iVar1 = tree->TreeNode_AllocData(param_5);
        reinterpret_cast<RBTreeNodeOverlay*>(param_4)->right = iVar1;
        iVar2 = tree->sentinel;
        if (param_4 != reinterpret_cast<RBTreeHeaderNodeOverlay*>(iVar2)->rightmost) {
            goto LAB_00461AA0;
        }
    }

    reinterpret_cast<RBTreeHeaderNodeOverlay*>(iVar2)->rightmost = iVar1;

LAB_00461AA0:
    reinterpret_cast<RBTreeNodeOverlay*>(iVar1)->left = 0;
    reinterpret_cast<RBTreeNodeOverlay*>(iVar1)->right = 0;
    reinterpret_cast<RBTreeNodeOverlay*>(iVar1)->parent = param_4;
    CMemoryAllocatorVariableSize__BalanceTree(iVar1, tree->sentinel + 4);
    tree->nodeCount = tree->nodeCount + 1;
    *param_2 = iVar1;
}