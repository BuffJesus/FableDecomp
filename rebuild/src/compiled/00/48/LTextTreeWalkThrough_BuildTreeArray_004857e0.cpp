#pragma optimize("s",on)

// Sub-object embedded at TreeNode+0x10.  Its cleanup is an out-of-line
// __fastcall member (this in ecx) taking no arguments -> emits
//   lea ecx,[esi+0x10]
//   call <Sub::Cleanup>
struct SubObject {
    void Cleanup();
};

struct TreeNode {
    char pad0[8];
    TreeNode* next;      // +0x8
    TreeNode* child;     // +0xc
    SubObject sub;       // +0x10
};

extern void __cdecl ProcessTreeNode(TreeNode* node);

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