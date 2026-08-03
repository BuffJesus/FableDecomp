#include <stdio.h>

struct Node {
    Node* pad0;   // +0x0
    Node* parent; // +0x4
    Node* left;   // +0x8
    Node* right;  // +0xc
    int   key;    // +0x10
};
struct Tree { Node* head; };

Node* __fastcall RBTree_FindNode_Dup(Tree* self, int /*edx*/, const int* pKey)
{
    Node* result = self->head;
    Node* node   = result->parent;
    if (node != 0) {
        int k = *pKey;
        do {
            if (node->key < k) {
                node = node->right;
            } else {
                result = node;
                node = node->left;
            }
        } while (node != 0);
    }
    return result;
}

// __fastcall/ret4 shim: this in ecx, one stack arg (const int*).
static Node* call_find(Tree* t, const int* pKey)
{
    Node* r;
    __asm {
        mov  ecx, t
        mov  eax, pKey
        push eax
        call RBTree_FindNode_Dup
        mov  r, eax
    }
    return r;
}

int main()
{
    // Build a small BST:
    //          n20
    //         /    \
    //       n10    n30
    // keys sorted: 10,20,30. lower_bound(k) = leftmost node with key >= k.
    Node n10 = {0,0,0,0,10};
    Node n20 = {0,0,0,0,20};
    Node n30 = {0,0,0,0,30};
    n20.left  = &n10;
    n20.right = &n30;

    Node head = {0,0,0,0,0};
    head.parent = &n20;   // root
    Tree tree; tree.head = &head;

    // lower_bound(20) -> n20
    int k;
    k = 20; if (call_find(&tree, &k) != &n20) { printf("FAIL lb20\n"); return 1; }
    // lower_bound(10) -> n10
    k = 10; if (call_find(&tree, &k) != &n10) { printf("FAIL lb10\n"); return 1; }
    // lower_bound(30) -> n30
    k = 30; if (call_find(&tree, &k) != &n30) { printf("FAIL lb30\n"); return 1; }
    // lower_bound(15) -> n20 (first key >= 15)
    k = 15; if (call_find(&tree, &k) != &n20) { printf("FAIL lb15\n"); return 1; }
    // lower_bound(25) -> n30
    k = 25; if (call_find(&tree, &k) != &n30) { printf("FAIL lb25\n"); return 1; }
    // lower_bound(5)  -> n10
    k = 5;  if (call_find(&tree, &k) != &n10) { printf("FAIL lb5\n");  return 1; }
    // lower_bound(35) -> head (no key >= 35, returns header/nil)
    k = 35; if (call_find(&tree, &k) != &head){ printf("FAIL lb35\n"); return 1; }

    printf("OK_0x005bacec\n");
    return 0;
}