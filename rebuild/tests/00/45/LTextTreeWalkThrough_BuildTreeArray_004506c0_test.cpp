#include <cstdio>

struct TreeNode {
    char pad0[8];
    TreeNode* next;   // +0x8
    TreeNode* child;  // +0xc
};

int g_visits = 0;
int g_order[64];
int g_n = 0;

void __cdecl ProcessTreeNode(TreeNode* node);

struct CTextTreeWalkThrough {
    void BuildTreeArray(TreeNode* node);
};

void CTextTreeWalkThrough::BuildTreeArray(TreeNode* node)
{
    if (node) {
        do {
            this->BuildTreeArray(node->child);
            TreeNode* next = node->next;
            ProcessTreeNode(node);
            node = next;
        } while (node);
    }
}

void __cdecl ProcessTreeNode(TreeNode* node)
{
    g_visits++;
    g_order[g_n++] = node->pad0[0];
}

int main()
{
    TreeNode A, B, C, D;
    A.pad0[0]='A'; A.next=&B; A.child=0;
    B.pad0[0]='B'; B.next=&D; B.child=&C;
    C.pad0[0]='C'; C.next=0;  C.child=0;
    D.pad0[0]='D'; D.next=0;  D.child=0;

    CTextTreeWalkThrough obj;
    obj.BuildTreeArray(&A);

    if (g_visits != 4) { std::printf("FAIL visits=%d\n", g_visits); return 1; }
    const char* exp = "ACBD";
    for (int i=0;i<4;i++) {
        if (g_order[i] != exp[i]) { std::printf("FAIL order[%d]=%c exp=%c\n", i, g_order[i], exp[i]); return 1; }
    }
    std::printf("LTextTreeWalkThrough_004506c0_TEST PASS\n");
    return 0;
}