#pragma optimize("s",on)
struct TreeNode { char pad0[8]; TreeNode* next; TreeNode* child; };
extern "C" void FreeNode(TreeNode* n); // reloc-masked free callee

struct CLTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};

void CLTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    if (node) {
        do {
            this->BuildTreeArray(node->child);
            TreeNode* nxt = node->next;
            FreeNode(node);
            node = nxt;
        } while (node);
    }
}