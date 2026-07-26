#include <cstdio>

struct TreeNode {
    char pad[8];
    TreeNode* next;
    TreeNode* child;
};

struct LTextTreeWalkThrough { char dummy; };
struct NodeArg { TreeNode* p; };

static int g_freed = 0;

extern "C" void __cdecl FreeNode(TreeNode* n)
{
    (void)n;
    g_freed++;
}

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

int main()
{
    TreeNode c; c.next = 0; c.child = 0;
    TreeNode a; a.next = 0; a.child = 0;
    TreeNode root; root.next = &a; root.child = &c;

    LTextTreeWalkThrough obj;
    NodeArg arg; arg.p = &root;
    LTextTreeWalkThrough_BuildTreeArray(&obj, arg);

    if (g_freed != 3) {
        std::printf("FAIL: freed=%d expected 3\n", g_freed);
        return 1;
    }
    std::printf("LTextTreeWalkThrough_004506ed_TEST PASS\n");
    return 0;
}