#include <stdio.h>

struct Node {
    unsigned int pad0;   // +0
    unsigned int pad4;   // +4
    Node* left;          // +8
    Node* right;         // +0xc
    unsigned int key;    // +0x10
};
struct Tree {
    Node* head;          // +0
};

Node* __fastcall RBTree_FindNode_Uint3(Tree* self, void* /*edx*/, unsigned int* pkey)
{
    Node* head = self->head;
    Node* cur = *(Node**)((char*)head + 4);
    Node* bound = head;
    if (cur) {
        do {
            if (cur->key >= *pkey) {
                bound = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        } while (cur);
    }
    return bound;
}

static Node* call_find(Tree* t, unsigned int* pkey)
{
    Node* r;
    __asm {
        mov  eax, pkey
        push eax
        mov  ecx, t
        call RBTree_FindNode_Uint3
        mov  r, eax
    }
    return r;
}

static Node* mk(Node* store, int idx, unsigned int key, Node* l, Node* r)
{
    store[idx].pad0 = 0;
    store[idx].pad4 = 0;
    store[idx].left = l;
    store[idx].right = r;
    store[idx].key = key;
    return &store[idx];
}

int main()
{
    Node nodes[8];
    Node* n10 = mk(nodes,0,10,0,0);
    Node* n30 = mk(nodes,1,30,0,0);
    Node* n50 = mk(nodes,2,50,0,0);
    Node* n70 = mk(nodes,3,70,0,0);
    Node* n20 = mk(nodes,4,20,n10,n30);
    Node* n60 = mk(nodes,5,60,n50,n70);
    Node* n40 = mk(nodes,6,40,n20,n60);

    Node head;
    head.pad0 = 0; head.pad4 = 0; head.left = 0; head.right = 0; head.key = 0;
    head.pad4 = (unsigned int)(unsigned long)n40;

    Tree tree;
    tree.head = &head;

    unsigned int k;
    Node* r;

    k = 30;  r = call_find(&tree, &k);
    if (r != n30) { printf("FAIL exact30 got %u\n", r?r->key:0); return 1; }

    k = 25;  r = call_find(&tree, &k);
    if (r != n30) { printf("FAIL lb25 got %u\n", r?r->key:0); return 1; }

    k = 10;  r = call_find(&tree, &k);
    if (r != n10) { printf("FAIL exact10 got %u\n", r?r->key:0); return 1; }

    k = 41;  r = call_find(&tree, &k);
    if (r != n50) { printf("FAIL lb41 got %u\n", r?r->key:0); return 1; }

    k = 70;  r = call_find(&tree, &k);
    if (r != n70) { printf("FAIL exact70 got %u\n", r?r->key:0); return 1; }

    k = 1000; r = call_find(&tree, &k);
    if (r != &head) { printf("FAIL end got %u\n", r?r->key:0); return 1; }

    printf("OK_0x005b1dfe all lower_bound cases pass\n");
    return 0;
}