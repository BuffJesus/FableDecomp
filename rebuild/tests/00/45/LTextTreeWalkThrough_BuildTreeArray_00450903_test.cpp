#include <cstdio>
struct TreeNode { int pad0; int pad4; TreeNode* next; TreeNode* child; };
static int g_freed = 0;
static int g_visits = 0;
void FreeNode(TreeNode* n){ g_freed++; }

struct LTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};

void LTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    while (node) {
        g_visits++;
        BuildTreeArray(node->child);
        TreeNode* nxt = node->next;
        FreeNode(node);
        node = nxt;
    }
}

int main(){
    TreeNode gc = {0,0,0,0};
    TreeNode c  = {0,0,0,&gc};
    TreeNode b  = {0,0,&c,0};
    TreeNode a  = {0,0,&b,0};
    LTextTreeWalkThrough obj;
    obj.BuildTreeArray(&a);
    if (g_freed == 4 && g_visits == 4) { std::printf("LTextTreeWalkThrough_00450903_TEST PASS\n"); return 0; }
    std::printf("FAIL freed=%d visits=%d\n", g_freed, g_visits); return 1;
}