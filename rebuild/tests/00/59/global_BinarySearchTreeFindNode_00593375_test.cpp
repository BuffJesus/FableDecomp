
#include <stdio.h>

struct Node {
    int   _pad0;    // +0x0
    Node* _up;      // +0x4  root when read from header
    Node* left;     // +0x8
    Node* right;    // +0xc
    int   key;      // +0x10
};
struct Tree { Node* head; };   // +0x0 header

// function-under-test: exact retail transcription (thiscall this=ecx, ret 4)
__declspec(naked) void FUT()
{
    __asm {
        mov  eax, [ecx]
        mov  ecx, [eax+4]
        test ecx, ecx
        je   L_done
        mov  edx, [esp+4]
        mov  edx, [edx]
    L_cmp:
        cmp  [ecx+0x10], edx
        jl   L_right
        mov  eax, ecx
        mov  ecx, [ecx+8]
        jmp  L_test
    L_right:
        mov  ecx, [ecx+0xc]
    L_test:
        test ecx, ecx
        jne  L_cmp
    L_done:
        ret  4
    }
}

// shim: ecx=this, push pkey; callee cleans 4 bytes (ret 4)
static Node* call_find(Tree* self, int* pkey)
{
    Node* r;
    __asm {
        mov  eax, pkey
        push eax
        mov  ecx, self
        call FUT
        mov  r, eax
    }
    return r;
}

int main()
{
    // Tree:        n20 (root)
    //             /       \
    //          n10        n30
    Node n10 = {0,0,0,0,10};
    Node n20 = {0,0,0,0,20};
    Node n30 = {0,0,0,0,30};
    n20.left = &n10; n20.right = &n30;
    Node header = {0,0,0,0,0};
    header._up = &n20;              // root at +4
    Tree t; t.head = &header;

    int k;
    // exact-key lower_bound hits
    k = 10; if (call_find(&t,&k) != &n10)    { printf("FAIL lb10\n"); return 1; }
    k = 20; if (call_find(&t,&k) != &n20)    { printf("FAIL lb20\n"); return 1; }
    k = 30; if (call_find(&t,&k) != &n30)    { printf("FAIL lb30\n"); return 1; }
    // between-key lower_bound (first node with key >= search)
    k = 15; if (call_find(&t,&k) != &n20)    { printf("FAIL lb15\n"); return 1; }
    k = 25; if (call_find(&t,&k) != &n30)    { printf("FAIL lb25\n"); return 1; }
    k = 5;  if (call_find(&t,&k) != &n10)    { printf("FAIL lb5\n");  return 1; }
    // above max -> nothing >= search, result stays at header
    k = 35; if (call_find(&t,&k) != &header) { printf("FAIL lb35\n"); return 1; }

    printf("OK_0x00593375\n");
    return 0;
}