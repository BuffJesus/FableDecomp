#pragma optimize("s",on)
struct Node {
    char pad0[8];
    Node* next;   /* +0x8 */
    Node* child;  /* +0xc */
};

extern void __cdecl ProcessNode(Node* n);

struct CFoo {
    void BuildTreeArray(Node* node);
};

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