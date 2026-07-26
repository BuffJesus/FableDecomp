#pragma optimize("s",on)
struct TreeNode {
    char pad0[8];
    TreeNode* next;   /* +0x8 */
    TreeNode* child;  /* +0xc */
};

extern void ProcessNode(TreeNode* n);

struct LTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};

void LTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    while (node) {
        this->BuildTreeArray(node->child);
        TreeNode* nxt = node->next;
        ProcessNode(node);
        node = nxt;
    }
}