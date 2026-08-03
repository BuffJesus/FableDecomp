#include <cstdio>

struct SubObject {
    int touched;
    void Cleanup();
};

struct TreeNode {
    char pad0[8];
    TreeNode* next;      // +0x8
    TreeNode* child;     // +0xc
    SubObject sub;       // +0x10
};

int g_visits = 0;
int g_cleaned = 0;
int g_order[64];
int g_n = 0;

void SubObject::Cleanup()
{
    this->touched = 1;
    g_cleaned++;
}

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
            node->sub.Cleanup();
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
    A.pad0[0]='A'; A.next=&B; A.child=0;  A.sub.touched=0;
    B.pad0[0]='B'; B.next=&D; B.child=&C; B.sub.touched=0;
    C.pad0[0]='C'; C.next=0;  C.child=0;  C.sub.touched=0;
    D.pad0[0]='D'; D.next=0;  D.child=0;  D.sub.touched=0;

    CTextTreeWalkThrough obj;
    obj.BuildTreeArray(&A);

    if (g_visits != 4) { std::printf("FAIL visits=%d\n", g_visits); return 1; }
    if (g_cleaned != 4) { std::printf("FAIL cleaned=%d\n", g_cleaned); return 1; }
    const char* exp = "ACBD";
    for (int i=0;i<4;i++) {
        if (g_order[i] != exp[i]) { std::printf("FAIL order[%d]=%c exp=%c\n", i, g_order[i], exp[i]); return 1; }
    }
    if (!A.sub.touched || !B.sub.touched || !C.sub.touched || !D.sub.touched) {
        std::printf("FAIL touched\n"); return 1;
    }
    std::printf("LTEXTTREEWALKTHROUGH_BUILDTREEARRAY_004857E0_TEST PASS\n");
    return 0;
}