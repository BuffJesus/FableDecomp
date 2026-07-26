#pragma optimize("s",on)
struct TreeNode;
struct NodePtr { TreeNode* p; };
struct TreeNode {
    char pad0[8];
    NodePtr sib;   /* +0x8 */
    NodePtr child; /* +0xc */
};
struct LTextTree { char dummy; };
void __cdecl ProcessTreeNode(TreeNode* n);
/* __fastcall: this in ecx, node pushed on stack (ret 0x4). NodePtr-by-value keeps edx free. */
void __fastcall LTextTree_BuildTreeArray(LTextTree* self, NodePtr node)
{
    while (node.p) {
        LTextTree_BuildTreeArray(self, node.p->child);
        NodePtr next = node.p->sib;
        ProcessTreeNode(node.p);
        node = next;
    }
}