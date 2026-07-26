#include <cstdio>

struct TreeNode {
    char pad0[8];
    TreeNode* next;   /* +0x8 */
    TreeNode* child;  /* +0xc */
};

static int g_visits = 0;
static TreeNode* g_order[16];

void ProcessNode(TreeNode* n) { g_order[g_visits++] = n; }

struct LTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};

void LTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    while (node) {
        this->BuildTreeArray(node->child);
        TreeNode* nxt = node->next;
        ProcessNode(node);
        node = nxt;
    }
}

int main() {
    TreeNode c1; c1.next = 0; c1.child = 0;
    TreeNode root; root.child = &c1; root.next = 0;
    TreeNode r2; r2.child = 0; r2.next = 0;
    root.next = &r2;

    LTextTreeWalkThrough obj;
    obj.BuildTreeArray(&root);

    if (g_visits == 3 && g_order[0] == &c1 && g_order[1] == &root && g_order[2] == &r2) {
        std::printf("LTextTreeWalkThrough_004507fb_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL visits=%d\n", g_visits);
    return 1;
}