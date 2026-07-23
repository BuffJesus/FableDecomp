#include <cstdio>
#include <cstdlib>

struct Node {
    Node* next;
    Node* prev;
};
struct Tree {
    Node* sentinel;
};

static int g_freed = 0;
static Node* g_freed_list[64];

extern "C" void FreeNode(Node* p) {
    g_freed_list[g_freed++] = p;
}

void __fastcall CGuiControlTreePane_SortTreeRecursively(Tree* self)
{
    Node* head = self->sentinel;
    Node* n = head->next;
    if (n != head) {
        do {
            Node* cur = n;
            n = n->next;
            FreeNode(cur);
        } while (n != self->sentinel);
    }
    self->sentinel->next = self->sentinel;
    self->sentinel->prev = self->sentinel;
}

int main() {
    // Build a circular doubly linked list with sentinel S and 3 nodes A,B,C
    Node S, A, B, C;
    // links via next(+0)/prev(+4): S -> A -> B -> C -> S
    S.next = &A; A.next = &B; B.next = &C; C.next = &S;
    S.prev = &C; C.prev = &B; B.prev = &A; A.prev = &S;
    Tree t; t.sentinel = &S;

    CGuiControlTreePane_SortTreeRecursively(&t);

    // Expect A,B,C freed in order (sentinel not freed)
    if (g_freed != 3) { std::printf("FAIL count=%d\n", g_freed); return 1; }
    if (g_freed_list[0]!=&A || g_freed_list[1]!=&B || g_freed_list[2]!=&C) {
        std::printf("FAIL order\n"); return 1;
    }
    // After clear, sentinel points to itself
    if (S.next != &S || S.prev != &S) { std::printf("FAIL reset\n"); return 1; }

    // Empty-list case: sentinel points to itself already
    Node S2; S2.next = &S2; S2.prev = &S2;
    Tree t2; t2.sentinel = &S2;
    g_freed = 0;
    CGuiControlTreePane_SortTreeRecursively(&t2);
    if (g_freed != 0) { std::printf("FAIL empty\n"); return 1; }
    if (S2.next != &S2 || S2.prev != &S2) { std::printf("FAIL empty reset\n"); return 1; }

    std::printf("CGuiControlTreePane_00450855_TEST PASS\n");
    return 0;
}