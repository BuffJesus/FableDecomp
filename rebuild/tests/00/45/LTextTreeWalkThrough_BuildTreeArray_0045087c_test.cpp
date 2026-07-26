#include <cstdio>
struct TreeNode;
struct NodePtr { TreeNode* p; };
struct TreeNode {
    char pad0[8];
    NodePtr sib;
    NodePtr child;
};
struct LTextTree { char dummy; };
int g_processed = 0;
void __cdecl ProcessTreeNode(TreeNode* n){ (void)n; g_processed++; }
void __fastcall LTextTree_BuildTreeArray(LTextTree* self, NodePtr node)
{
    while (node.p) {
        LTextTree_BuildTreeArray(self, node.p->child);
        NodePtr next = node.p->sib;
        ProcessTreeNode(node.p);
        node = next;
    }
}
static TreeNode* mk(){ TreeNode* n=new TreeNode(); n->sib.p=0; n->child.p=0; return n; }
int main(){
    TreeNode* A=mk();
    TreeNode* B=mk();
    TreeNode* C=mk();
    A->sib.p=B; A->child.p=C;
    LTextTree t;
    NodePtr a; a.p=A;
    LTextTree_BuildTreeArray(&t, a);
    if (g_processed != 3) { std::printf("FAIL processed=%d\n", g_processed); return 1; }
    g_processed=0;
    NodePtr z; z.p=0;
    LTextTree_BuildTreeArray(&t, z);
    if (g_processed != 0) { std::printf("FAIL null processed=%d\n", g_processed); return 1; }
    std::printf("LTextTreeWalkThrough_0045087c_TEST PASS\n");
    return 0;
}