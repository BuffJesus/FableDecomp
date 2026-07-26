#pragma optimize("s",on)
struct TreeNode { int pad0; int pad4; TreeNode* next; TreeNode* child; };
extern void FreeNode(TreeNode* n);

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