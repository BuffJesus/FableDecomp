#pragma optimize("s",on)
struct TreeNode {
    char pad0[8];
    TreeNode* next;   // +0x8
    TreeNode* child;  // +0xc
};

extern void __cdecl ProcessTreeNode(TreeNode* node);

struct CTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};

void CTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    if (node) {
        do {
            this->BuildTreeArray(node->child);
            TreeNode* next = node->next;
            ProcessTreeNode(node);
            node = next;
        } while (node);
    }
}