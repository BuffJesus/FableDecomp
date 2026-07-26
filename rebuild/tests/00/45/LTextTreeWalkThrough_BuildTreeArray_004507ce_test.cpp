#include <cstdio>

struct TreeNode {
    char pad0[8];
    TreeNode* next;   // +0x8
    TreeNode* child;  // +0xc
};

int g_freed = 0;
void FreeNode(TreeNode* n) { (void)n; g_freed++; }

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

int main() {
    TreeNode c; c.next = 0; c.child = 0;
    TreeNode a; a.next = 0; a.child = 0;
    TreeNode root; root.next = &a; root.child = &c;

    LTextTreeWalkThrough w;
    w.BuildTreeArray(&root);

    if (g_freed == 3) {
        std::printf("LTextTreeWalkThrough_004507ce_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL freed=%d\n", g_freed);
    return 1;
}