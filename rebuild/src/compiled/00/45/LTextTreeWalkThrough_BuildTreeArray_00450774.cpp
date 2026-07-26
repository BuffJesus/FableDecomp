#pragma optimize("s",on)
struct TreeNode { char pad0[8]; TreeNode* next; TreeNode* child; };
extern "C" void __cdecl tree_free(void* p);
struct LTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};
void LTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    while (node) {
        this->BuildTreeArray(node->child);
        TreeNode* nxt = node->next;
        tree_free(node);
        node = nxt;
    }
}