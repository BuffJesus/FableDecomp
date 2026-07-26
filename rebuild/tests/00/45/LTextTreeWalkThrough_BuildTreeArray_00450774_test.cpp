#include <cstdio>
#include <cstdlib>
struct TreeNode { char pad0[8]; TreeNode* next; TreeNode* child; };
static int g_freed = 0;
extern "C" void __cdecl tree_free(void* p){ g_freed++; std::free(p); }
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
static TreeNode* mk(TreeNode* next, TreeNode* child){ TreeNode* n=(TreeNode*)std::calloc(1,sizeof(TreeNode)); n->next=next; n->child=child; return n; }
int main(){
    TreeNode* leaf1 = mk(0,0);
    TreeNode* leaf2 = mk(0,0);
    TreeNode* second = mk(0, leaf2);
    TreeNode* root = mk(second, leaf1);
    LTextTreeWalkThrough obj;
    obj.BuildTreeArray(root);
    if (g_freed != 4) { std::printf("FAIL freed=%d\n", g_freed); return 1; }
    obj.BuildTreeArray(0);
    std::printf("LTextTreeWalkThrough_00450774_TEST PASS\n");
    return 0;
}