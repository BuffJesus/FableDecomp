#include <stdio.h>

struct Node {
    unsigned int pad0;
    unsigned int pad4;
    Node* left;          // +8
    Node* right;         // +0xc
    int key;             // +0x10
};
struct Tree {
    Node* head;          // +0
};

Node* __fastcall RBTree_FindLT_Reverse(Tree* self, void* /*edx*/, int* pkey)
{
    Node* result = self->head;
    Node* cur = *(Node**)((char*)result + 4);
    while (cur) {
        if (*pkey < cur->key) {
            result = cur;
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return result;
}

// __fastcall shim: this=ecx, one stack arg (pkey)
static Node* call_find(Tree* self, int* pkey)
{
    Node* r;
    __asm {
        mov  ecx, self
        mov  eax, pkey
        push eax
        call RBTree_FindLT_Reverse   // callee pops arg (ret 4)
        mov  r, eax
    }
    return r;
}

int main()
{
    Node n10, n20, n30, header;
    n10.key = 10; n10.left = 0; n10.right = 0;
    n30.key = 30; n30.left = 0; n30.right = 0;
    n20.key = 20; n20.left = &n10; n20.right = &n30;
    // header: [head+4] (pad4) points at root
    header.pad4 = (unsigned int)&n20;

    Tree t; t.head = &header;

    int k;
    k = 15; Node* r15 = call_find(&t, &k);   // expect n20
    k = 25; Node* r25 = call_find(&t, &k);   // expect n30
    k = 100; Node* r100 = call_find(&t, &k); // expect header (no key > 100)
    k = 5;  Node* r5  = call_find(&t, &k);   // expect n10 (smallest key >5 is 10)

    int ok = (r15 == &n20) && (r25 == &n30) && (r100 == &header) && (r5 == &n10);
    if (ok) {
        printf("OK_0x005bad36 r15=%d r25=%d r5=%d\n", r15->key, r25->key, r5->key);
        return 0;
    }
    printf("FAIL r15=%p r25=%p r100=%p r5=%p\n", (void*)r15, (void*)r25, (void*)r100, (void*)r5);
    return 1;
}