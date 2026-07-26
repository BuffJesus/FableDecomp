#include <cstdio>

struct Node {
    char pad0[8];
    Node* next;
    int   value;
};

static int g_sum = 0;
static int g_freed = 0;

struct CTree {
    void Handle(int v);
};
void CTree::Handle(int v) { g_sum += v; }

void __cdecl FreeNode(Node* n) { (void)n; g_freed++; }

void __fastcall LTextTreeWalkThrough_BuildTreeArray(CTree* self, void* edx, Node* node)
{
    (void)edx;
    while (node != 0) {
        self->Handle(node->value);
        Node* nxt = node->next;
        FreeNode(node);
        node = nxt;
    }
}

int main() {
    Node c; c.next = 0; c.value = 3;
    Node b; b.next = &c; b.value = 2;
    Node a; a.next = &b; a.value = 1;
    CTree t;
    LTextTreeWalkThrough_BuildTreeArray(&t, 0, &a);
    if (g_sum == 6 && g_freed == 3) {
        std::printf("LTextTreeWalkThrough_0045071a_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL sum=%d freed=%d\n", g_sum, g_freed);
    return 1;
}