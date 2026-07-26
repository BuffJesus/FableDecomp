#include <cstdio>

struct TreeNode {
    void* pad0;
    void* pad4;
    TreeNode* next;
    TreeNode* child;
};

int g_freed = 0;
int g_children = 0;

struct CTreeWalk {
    void BuildTreeArray(TreeNode* node);
    void AddChild(TreeNode* c){ if(c) g_children++; }
};

void FreeTreeNode(TreeNode* node){ g_freed++; }

void CTreeWalk::BuildTreeArray(TreeNode* node) {
    while (node) {
        this->BuildTreeArray(node->child);
        TreeNode* next = node->next;
        FreeTreeNode(node);
        node = next;
    }
}

int main(){
    /* build: root -> a(next) -> b(next); a has child c */
    TreeNode c; c.pad0=0; c.pad4=0; c.next=0; c.child=0;
    TreeNode a; a.pad0=0; a.pad4=0; a.next=0; a.child=&c;
    TreeNode root; root.pad0=0; root.pad4=0; root.next=&a; root.child=0;
    CTreeWalk w;
    w.BuildTreeArray(&root);
    if (g_freed == 3) { std::printf("LTextTreeWalkThrough_004507a1_TEST PASS\n"); return 0; }
    std::printf("FAIL freed=%d\n", g_freed);
    return 1;
}