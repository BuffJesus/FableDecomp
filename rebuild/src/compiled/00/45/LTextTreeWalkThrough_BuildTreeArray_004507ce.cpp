#pragma optimize("s",on)
struct TreeNode {
    char pad0[8];
    TreeNode* next;   // +0x8
    TreeNode* child;  // +0xc
};

void FreeNode(TreeNode* n);

struct LTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};

void LTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    while (node) {
        BuildTreeArray(node->child);
        TreeNode* nxt = node->next;
        FreeNode(node);
        node = nxt;
    }
}