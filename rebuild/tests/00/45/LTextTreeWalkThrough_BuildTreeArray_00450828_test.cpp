#include <cstdio>
#include <cstdlib>

struct Node {
    char pad[8];
    Node* next;   // +0x8
    Node* child;  // +0xc
};

static int g_freed = 0;
extern "C" void __cdecl freeNode(Node* p) { g_freed++; std::free(p); }

struct CTree {
    void BuildTreeArray(Node* n);
};

void CTree::BuildTreeArray(Node* n)
{
    if (n) {
        do {
            this->BuildTreeArray(n->child);
            Node* next = n->next;
            freeNode(n);
            n = next;
        } while (n);
    }
}

static Node* mk(Node* next, Node* child) {
    Node* p = (Node*)std::malloc(sizeof(Node));
    p->next = next; p->child = child;
    return p;
}

int main() {
    Node* c1 = mk(0, 0);
    Node* c2 = mk(c1, 0);
    Node* n2 = mk(0, 0);
    Node* n1 = mk(n2, c2);
    CTree t;
    t.BuildTreeArray(n1);
    if (g_freed == 4) { std::printf("LTextTreeWalkThrough_00450828_TEST PASS\n"); return 0; }
    std::printf("FAIL freed=%d\n", g_freed);
    return 1;
}