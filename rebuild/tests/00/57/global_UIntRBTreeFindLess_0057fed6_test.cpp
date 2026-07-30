#include <cstdio>

struct RBNode {
    void*        _pad0;
    void*        _pad4;
    RBNode*      left;
    RBNode*      right;
    unsigned int key;
};
struct RBHead { RBNode* _dummy; RBNode* root; };
struct RBTree { RBHead* head; };

// __declspec(naked) transcription mirroring retail (this=ecx, one stack arg, ret 4).
__declspec(naked) void UInt_RBTreeFindLess()
{
    __asm {
        mov  eax, dword ptr [ecx]
        mov  ecx, dword ptr [eax+0x4]
        test ecx, ecx
        je   done
        mov  edx, dword ptr [esp+0x4]
        mov  edx, dword ptr [edx]
    loop_top:
        cmp  edx, dword ptr [ecx+0x10]
        jae  go_right
        mov  eax, ecx
        mov  ecx, dword ptr [ecx+0x8]
        jmp  test_node
    go_right:
        mov  ecx, dword ptr [ecx+0xc]
    test_node:
        test ecx, ecx
        jne  loop_top
    done:
        ret  0x4
    }
}

// this=ecx, one stack arg (pointer to key), callee cleans 4 bytes (ret 4).
static RBNode* call_find(RBTree* self, unsigned int* pkey)
{
    RBNode* r;
    __asm {
        mov  eax, pkey
        push eax
        mov  ecx, self
        call UInt_RBTreeFindLess
        mov  r, eax
    }
    return r;
}

int main()
{
    RBNode n10={0,0,0,0,10}, n20={0,0,0,0,20}, n30={0,0,0,0,30}, n50={0,0,0,0,50};
    n30.left=&n10; n30.right=&n50;
    n10.left=0;    n10.right=&n20;
    RBHead head; head._dummy=0; head.root=&n30;
    RBTree tree; tree.head=&head;

    unsigned int k;
    RBNode* r;

    k = 15; r = call_find(&tree, &k);            // smallest key >15 is 20
    if (r != &n20) { printf("FAIL k=15 got key=%u\n", r?r->key:0); return 1; }
    k = 25; r = call_find(&tree, &k);            // smallest key >25 is 30
    if (r != &n30) { printf("FAIL k=25\n"); return 1; }
    k = 5;  r = call_find(&tree, &k);            // smallest key >5 is 10
    if (r != &n10) { printf("FAIL k=5\n"); return 1; }
    k = 50; r = call_find(&tree, &k);            // nothing >50 -> head pointer
    if (r != (RBNode*)&head) { printf("FAIL k=50\n"); return 1; }
    k = 30; r = call_find(&tree, &k);            // smallest key >30 is 50
    if (r != &n50) { printf("FAIL k=30\n"); return 1; }

    printf("OK_0x0057fed6\n");
    return 0;
}