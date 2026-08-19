#pragma optimize("s",on)
struct CInner {
    void Destroy(int flag);  // __fastcall member at node+0x10
};

struct CTextTreeNode {
    char pad0[8];
    CTextTreeNode* next;    // +0x8
    CTextTreeNode* child;   // +0xc
    CInner inner;           // +0x10
};

extern "C" void __cdecl FreeTreeNode(void* p);

class LTextTreeWalkThrough {
public:
    void BuildTreeArray(CTextTreeNode* node);
};

void LTextTreeWalkThrough::BuildTreeArray(CTextTreeNode* node)
{
    while (node) {
        this->BuildTreeArray(node->child);
        CTextTreeNode* nxt = node->next;
        node->inner.Destroy(0);
        FreeTreeNode(node);
        node = nxt;
    }
}