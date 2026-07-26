#pragma optimize("s",on)
struct TreeNode {
    void* pad0;
    void* pad4;
    TreeNode* next; /* +0x8 */
    TreeNode* child; /* +0xc */
};

struct CTreeWalk {
    void BuildTreeArray(TreeNode* node);
};

extern void FreeTreeNode(TreeNode* node);

void CTreeWalk::BuildTreeArray(TreeNode* node) {
    while (node) {
        this->BuildTreeArray(node->child);
        TreeNode* next = node->next;
        FreeTreeNode(node);
        node = next;
    }
}