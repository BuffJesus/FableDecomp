#include <cstdio>

struct Node {
    char pad0[8];
    Node* next;   /* +0x8 */
    Node* child;  /* +0xc */
};

void __cdecl ProcessNode(Node* n);

struct CFoo {
    void BuildTreeArray(Node* node);
};

static int g_visited = 0;
static Node* g_order[16];

void __cdecl ProcessNode(Node* n)
{
    g_order[g_visited++] = n;
}

void CFoo::BuildTreeArray(Node* node)
{
    while (node)
    {
        BuildTreeArray(node->child);
        Node* nxt = node->next;
        ProcessNode(node);
        node = nxt;
    }
}

int main()
{
    Node a, b, c, d;
    a.next = &b; a.child = &c;
    b.next = 0;  b.child = 0;
    c.next = &d; c.child = 0;
    d.next = 0;  d.child = 0;

    CFoo self;
    self.BuildTreeArray(&a);

    if (g_visited != 4) { std::printf("FAIL count %d\n", g_visited); return 1; }
    if (g_order[0]!=&c || g_order[1]!=&d || g_order[2]!=&a || g_order[3]!=&b) {
        std::printf("FAIL order\n"); return 1;
    }
    std::printf("LTextTreeWalkThrough_00450930_TEST PASS\n");
    return 0;
}