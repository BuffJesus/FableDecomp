#pragma optimize("s",on)
struct TreeNode {
    char pad[8];
    TreeNode* next;   /* +0x8 */
    TreeNode* child;  /* +0xc */
};

struct LTextTreeWalkThrough { char dummy; };

/* one pointer-sized struct passed BY VALUE -> stays on the stack under fastcall,
   so ecx=this, single stack arg, ret 4 (matches the __fastcall retail). */
struct NodeArg { TreeNode* p; };

extern "C" void __cdecl FreeNode(TreeNode* n);

void __fastcall LTextTreeWalkThrough_BuildTreeArray(LTextTreeWalkThrough* self, NodeArg arg)
{
    TreeNode* node = arg.p;
    while (node)
    {
        NodeArg ca; ca.p = node->child;
        LTextTreeWalkThrough_BuildTreeArray(self, ca);
        TreeNode* nxt = node->next;
        FreeNode(node);
        node = nxt;
    }
}