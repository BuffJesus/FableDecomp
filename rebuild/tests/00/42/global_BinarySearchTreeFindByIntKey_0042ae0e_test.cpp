
#include <stdio.h>

struct Node { Node* m0; Node* parent; Node* left; Node* right; int key; };
struct Tree { Node* head; };

Node* __fastcall BinarySearchTree_FindByIntKey(Tree* self, void* /*edx*/, int* pkey)
{
    Node* result = self->head;
    Node* cur = result->parent;
    while (cur) {
        int k = *pkey;
        if (cur->key >= k) { result = cur; cur = cur->left; }
        else { cur = cur->right; }
    }
    return result;
}

static Node* call_find(Tree* t, int* pkey) {
    Node* r;
    __asm {
        mov  eax, pkey
        push eax
        mov  ecx, t
        call BinarySearchTree_FindByIntKey
        mov  r, eax
    }
    return r;
}

int main() {
    Node n10 = {0,0,0,0,10};
    Node n20 = {0,0,0,0,20};
    Node n30 = {0,0,0,0,30};
    Node head = {0,0,0,0,0};
    n20.left = &n10; n20.right = &n30;
    head.parent = &n20;
    Tree t; t.head = &head;

    int k;
    k = 20; Node* r1 = call_find(&t, &k);
    k = 10; Node* r2 = call_find(&t, &k);
    k = 30; Node* r3 = call_find(&t, &k);
    k = 25; Node* r4 = call_find(&t, &k);
    k = 100; Node* r5 = call_find(&t, &k);

    if (r1 != &n20) { printf("FAIL r1 key=%d\n", r1?r1->key:-1); return 1; }
    if (r2 != &n10) { printf("FAIL r2 key=%d\n", r2?r2->key:-1); return 1; }
    if (r3 != &n30) { printf("FAIL r3 key=%d\n", r3?r3->key:-1); return 1; }
    if (r4 != &n30) { printf("FAIL r4 key=%d\n", r4?r4->key:-1); return 1; }
    if (r5 != &head){ printf("FAIL r5 key=%d\n", r5?r5->key:-1); return 1; }

    printf("OK_0x0042ae0e\n");
    return 0;
}