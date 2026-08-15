#include <cstdio>
#include <cstdlib>

static int g_destroyCount = 0;
static int g_freeCount = 0;

struct CInner {
    int marker;
    void Destroy(int flag);
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

void CInner::Destroy(int flag) {
    (void)flag;
    g_destroyCount++;
}

extern "C" void __cdecl FreeTreeNode(void* p) {
    g_freeCount++;
    std::free(p);
}

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

static CTextTreeNode* mk(CTextTreeNode* next, CTextTreeNode* child) {
    CTextTreeNode* n = (CTextTreeNode*)std::calloc(1, sizeof(CTextTreeNode));
    n->next = next;
    n->child = child;
    return n;
}

int main() {
    // Build a small tree:
    //   root -> next -> (null)
    //   root has child -> grandchild
    // Total nodes: root, sibling, child, grandchild = 4
    CTextTreeNode* grandchild = mk(0, 0);
    CTextTreeNode* child = mk(0, grandchild);
    CTextTreeNode* sibling = mk(0, 0);
    CTextTreeNode* root = mk(sibling, child);

    LTextTreeWalkThrough walker;
    walker.BuildTreeArray(root);

    if (g_freeCount == 4 && g_destroyCount == 4) {
        std::printf("LTextTreeWalkThrough_00451b4b_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL free=%d destroy=%d\n", g_freeCount, g_destroyCount);
    return 1;
}