#include <cstdio>

struct Node {
    int   _pad0;   // +0x0
    Node* left;    // +0x4
    Node* rightC;  // +0x8 (child via +0x8)
    Node* rightD;  // +0xc (child via +0xc)
    int   key;     // +0x10
};
struct Tree { Node* head; };

__declspec(naked) Node* __fastcall BinaryTree_Find(Tree*, void*, const int*){
    __asm {
        mov  eax, [ecx]
        mov  ecx, [eax+4]
        test ecx, ecx
        je   L_end
        mov  edx, [esp+4]
        mov  edx, [edx]
    L_loop:
        cmp  [ecx+0x10], edx
        jl   L_right
        mov  eax, ecx
        mov  ecx, [ecx+8]
        jmp  L_test
    L_right:
        mov  ecx, [ecx+0xc]
    L_test:
        test ecx, ecx
        jne  L_loop
    L_end:
        ret  4
    }
}

// __fastcall shim: this=ecx, one stack arg (keyp), ret 4
static Node* Call(Tree* self, const int* keyp)
{
    Node* r;
    __asm {
        mov  ecx, self
        mov  eax, keyp
        push eax
        call BinaryTree_Find
        mov  r, eax
    }
    return r;
}

int main()
{
    // STL-style lower_bound over a BST: returns last node with node->key >= key,
    // or the head sentinel (end) if none.
    Node n10 = {0,0,0,0,10};
    Node n20 = {0,0,0,0,20};
    Node n30 = {0,0,0,0,30};
    Node n40 = {0,0,0,0,40};
    Node n50 = {0,0,0,0,50};
    Node head = {0,0,0,0,0};

    //        30
    //       /  \
    //      20   40
    //     /       \
    //    10        50
    // walk: +0x8 when key <= node (toward smaller candidate), +0xc when node < key.
    n30.rightC = &n20; n30.rightD = &n40;
    n20.rightC = &n10; n20.rightD = 0;
    n40.rightC = 0;    n40.rightD = &n50;
    n10.rightC = 0;    n10.rightD = 0;
    n50.rightC = 0;    n50.rightD = 0;

    head.left = &n30;
    Tree t; t.head = &head;

    int k;
    Node* r;

    k = 30; r = Call(&t, &k);
    if (r != &n30) { printf("FAIL hit30 got %d\n", r?r->key:-1); return 1; }

    k = 10; r = Call(&t, &k);
    if (r != &n10) { printf("FAIL hit10 got %d\n", r?r->key:-1); return 1; }

    k = 50; r = Call(&t, &k);
    if (r != &n50) { printf("FAIL hit50 got %d\n", r?r->key:-1); return 1; }

    k = 25; r = Call(&t, &k);
    if (r != &n30) { printf("FAIL lb25 got %d\n", r?r->key:-1); return 1; }

    k = 15; r = Call(&t, &k);
    if (r != &n20) { printf("FAIL lb15 got %d\n", r?r->key:-1); return 1; }

    k = 999; r = Call(&t, &k);
    if (r != &head) { printf("FAIL lbend got %d\n", r?r->key:-1); return 1; }

    k = 5; r = Call(&t, &k);
    if (r != &n10) { printf("FAIL lbmin got %d\n", r?r->key:-1); return 1; }

    printf("OK_0x0042ae33\n");
    return 0;
}