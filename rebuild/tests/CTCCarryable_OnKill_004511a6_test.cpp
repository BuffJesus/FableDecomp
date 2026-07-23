#include <cstdio>

struct Node {
    Node* v0;
    int   v4;
    Node* v8;
    Node* vc;
};
struct List {
    Node* head;
    int   node;
};

static int g_freed = 0;
extern "C" void __stdcall FreeNode(int p) { g_freed = p; }

void __fastcall CTCCarryable_OnKill(List* self)
{
    if (self->node) {
        Node* n = self->head;
        FreeNode(n->v4);
        self->head->v8 = self->head;
        self->head->v4 &= 0;
        self->head->vc = self->head;
        self->node &= 0;
    }
}

int main() {
    Node n;
    n.v0 = (Node*)0; n.v4 = 0x1234; n.v8 = (Node*)0; n.vc = (Node*)0;
    List L;
    L.head = &n;
    L.node = 0xDEAD;

    CTCCarryable_OnKill(&L);

    if (g_freed != 0x1234) { std::printf("FAIL freed\n"); return 1; }
    if (n.v8 != &n) { std::printf("FAIL v8\n"); return 1; }
    if (n.v4 != 0) { std::printf("FAIL v4\n"); return 1; }
    if (n.vc != &n) { std::printf("FAIL vc\n"); return 1; }
    if (L.node != 0) { std::printf("FAIL node\n"); return 1; }

    List L2;
    L2.head = (Node*)0;
    L2.node = 0;
    g_freed = 0xBEEF;
    CTCCarryable_OnKill(&L2);
    if (g_freed != 0xBEEF) { std::printf("FAIL skip\n"); return 1; }
    if (L2.node != 0) { std::printf("FAIL node2\n"); return 1; }

    std::printf("CTCCarryable_004511a6_TEST PASS\n");
    return 0;
}