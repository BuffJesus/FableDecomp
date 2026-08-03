#include <stdio.h>

struct Node {
    Node* left;    // +0x0
    Node* parent;  // +0x4
    Node* right;   // +0x8  (taken when node->key >= *keyptr : lesser-or-equal subtree)
    Node* rr;      // +0xc  (taken when node->key <  *keyptr : greater subtree)
    unsigned int key; // +0x10
};

struct Tree { Node* head; }; // +0x0 head sentinel; root at head->parent(+4)

// Function under test: exact retail transcription (this=ecx, one stack arg, ret 4).
__declspec(naked) void BinarySearchTree_FindEqualOrGreater()
{
    __asm {
        mov     eax, dword ptr [ecx]
        mov     ecx, dword ptr [eax+4]
        test    ecx, ecx
        je      done
        mov     edx, dword ptr [esp+4]
        mov     edx, dword ptr [edx]
    loop_top:
        cmp     dword ptr [ecx+0x10], edx
        jb      go_right
        mov     eax, ecx
        mov     ecx, dword ptr [ecx+8]
        jmp     test_node
    go_right:
        mov     ecx, dword ptr [ecx+0xc]
    test_node:
        test    ecx, ecx
        jne     loop_top
    done:
        ret     4
    }
}

// Shim: this=ecx, keyptr pushed as the single stack arg (callee does ret 4).
static Node* callit(Tree* t, unsigned int* k)
{
    Node* r;
    __asm {
        mov ecx, t
        mov eax, k
        push eax
        call BinarySearchTree_FindEqualOrGreater
        mov r, eax
    }
    return r;
}

int main()
{
    //            root(50)
    //           /        \
    //        (30)        (70)
    // right(+8)=lesser-or-equal branch ; rr(+0xc)=greater branch
    Node head, root, n30, n70;
    head.left = 0; head.parent = &root; head.right = 0; head.rr = 0; head.key = 0;

    root.key = 50; root.left = 0; root.parent = &head;
    root.right = &n30;   // +0x8
    root.rr    = &n70;   // +0xc

    n30.key = 30; n30.left = 0; n30.parent = &root; n30.right = 0; n30.rr = 0;
    n70.key = 70; n70.left = 0; n70.parent = &root; n70.right = 0; n70.rr = 0;

    Tree tree; tree.head = &head;

    unsigned int t;
    t = 50; Node* r1 = callit(&tree, &t);  // exact -> root
    t = 40; Node* r2 = callit(&tree, &t);  // >=40 smallest is 50 -> root
    t = 30; Node* r3 = callit(&tree, &t);  // exact -> n30
    t = 100; Node* r4 = callit(&tree, &t); // none >= 100 -> head sentinel
    t = 60; Node* r5 = callit(&tree, &t);  // >=60 smallest is 70 -> n70
    t = 25; Node* r6 = callit(&tree, &t);  // >=25 smallest is 30 -> n30

    int ok = 1;
    if (r1 != &root) { printf("FAIL r1 key=%u\n", r1 ? r1->key : 0); ok = 0; }
    if (r2 != &root) { printf("FAIL r2 key=%u\n", r2 ? r2->key : 0); ok = 0; }
    if (r3 != &n30)  { printf("FAIL r3 key=%u\n", r3 ? r3->key : 0); ok = 0; }
    if (r4 != &head) { printf("FAIL r4 (expected head sentinel)\n"); ok = 0; }
    if (r5 != &n70)  { printf("FAIL r5 key=%u\n", r5 ? r5->key : 0); ok = 0; }
    if (r6 != &n30)  { printf("FAIL r6 key=%u\n", r6 ? r6->key : 0); ok = 0; }

    if (ok) { printf("OK_0x0042ad6c lower_bound over hand-built tree: all cases passed\n"); return 0; }
    return 1;
}