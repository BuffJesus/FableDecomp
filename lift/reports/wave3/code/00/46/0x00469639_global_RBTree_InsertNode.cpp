class CCreatureActionBase;
class CCreatureAction_NavToPos {
public:
    CCreatureActionBase* Clone(int* param_5);
};

extern "C" void CMemoryAllocatorVariableSize__BalanceTree(
    CCreatureActionBase* node,
    void* root_link
);

struct RBTreeHeaderOverlay {
    std::byte pad00[0x4];
    CCreatureActionBase* root;
    CCreatureActionBase* leftmost;
    CCreatureActionBase* rightmost;
};

struct RBTreeNodeOverlay {
    std::byte pad00[0x4];
    CCreatureActionBase* parent;
    CCreatureActionBase* left;
    CCreatureActionBase* right;
    int value_at_10;
};

struct RBTreeOwnerOverlay {
    RBTreeHeaderOverlay* header;
    int count;
};

static_assert(offsetof(RBTreeHeaderOverlay, root) == 0x4);
static_assert(offsetof(RBTreeHeaderOverlay, leftmost) == 0x8);
static_assert(offsetof(RBTreeHeaderOverlay, rightmost) == 0xC);

static_assert(offsetof(RBTreeNodeOverlay, parent) == 0x4);
static_assert(offsetof(RBTreeNodeOverlay, left) == 0x8);
static_assert(offsetof(RBTreeNodeOverlay, right) == 0xC);
static_assert(offsetof(RBTreeNodeOverlay, value_at_10) == 0x10);

static_assert(offsetof(RBTreeOwnerOverlay, header) == 0x0);
static_assert(offsetof(RBTreeOwnerOverlay, count) == 0x4);

void __thiscall RBTree_InsertNode(
    CCreatureAction_NavToPos* param_1,
    CCreatureActionBase** param_2,
    int param_3,
    CCreatureActionBase* param_4,
    int* param_5,
    int param_6
) {
    auto* const tree = reinterpret_cast<RBTreeOwnerOverlay*>(param_1);
    auto* const header = tree->header;
    auto* const insert_pos = reinterpret_cast<RBTreeNodeOverlay*>(param_4);

    CCreatureActionBase* pCVar1;
    RBTreeHeaderOverlay* iVar2;

    if ((param_4 == reinterpret_cast<CCreatureActionBase*>(header)) ||
        ((param_6 == 0) && ((param_3 != 0) || (*param_5 < insert_pos->value_at_10)))) {
        pCVar1 = param_1->Clone(param_5);
        insert_pos->left = pCVar1;
        iVar2 = tree->header;
        if (param_4 != reinterpret_cast<CCreatureActionBase*>(iVar2)) {
            if (param_4 == iVar2->leftmost) {
                iVar2->leftmost = pCVar1;
            }
            goto LAB_00469699;
        }
        iVar2->root = pCVar1;
        iVar2 = tree->header;
    } else {
        pCVar1 = param_1->Clone(param_5);
        insert_pos->right = pCVar1;
        iVar2 = tree->header;
        if (param_4 != iVar2->rightmost) {
            goto LAB_00469699;
        }
    }

    iVar2->rightmost = pCVar1;

LAB_00469699:
    auto* const new_node = reinterpret_cast<RBTreeNodeOverlay*>(pCVar1);
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = param_4;

    CMemoryAllocatorVariableSize__BalanceTree(
        pCVar1,
        reinterpret_cast<std::byte*>(tree->header) + 0x4
    );

    tree->count = tree->count + 1;
    *param_2 = pCVar1;
}