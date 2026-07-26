#include <cstdio>
#include <cstdlib>

struct TreeNode { char pad0[8]; TreeNode* next; TreeNode* child; };

static int g_freed = 0;
extern "C" void FreeNode(TreeNode* n) { (void)n; ++g_freed; }

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

static TreeNode* mk(TreeNode* next, TreeNode* child) {
    TreeNode* n = (TreeNode*)std::calloc(1, sizeof(TreeNode));
    n->next = next; n->child = child; return n;
}

int main() {
    TreeNode* c1 = mk(0, 0);
    TreeNode* c2 = mk(0, 0);
    TreeNode* s2 = mk(0, c2);
    TreeNode* s1 = mk(s2, c1);
    CLTextTreeWalkThrough obj;
    obj.BuildTreeArray(s1);
    if (g_freed != 4) { std::printf("FAIL freed=%d\n", g_freed); return 1; }
    obj.BuildTreeArray(0);
    if (g_freed != 4) { std::printf("FAIL null-case freed=%d\n", g_freed); return 1; }
    std::printf("LTextTreeWalkThrough_00450747_TEST PASS\n");
    return 0;
}