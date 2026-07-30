#include <stdio.h>

__declspec(naked) void* RBTree_FindNode_Int(void* keyPtr)
{
    (void)keyPtr;
    __asm {
        mov eax, dword ptr [ecx]
        mov ecx, dword ptr [eax+4]
        test ecx, ecx
        je done
        mov edx, dword ptr [esp+4]
        mov edx, dword ptr [edx]
    loop_top:
        cmp dword ptr [ecx+0x10], edx
        jl go_left
        mov eax, ecx
        mov ecx, dword ptr [ecx+8]
        jmp after
    go_left:
        mov ecx, dword ptr [ecx+0xc]
    after:
        test ecx, ecx
        jne loop_top
    done:
        ret 4
    }
}


struct Node {
    void* pad0;   /* +0x0 */
    void* pad4;   /* +0x4  (== head->root slot) */
    Node* right;  /* +0x8 */
    Node* left;   /* +0xc */
    int   key;    /* +0x10 */
};

struct Header { Node* head; };  /* [this+0] -> head node */

static void* callit(Header* s, int* k)
{
    void* r;
    __asm {
        mov ecx, s
        mov eax, k
        push eax
        call RBTree_FindNode_Int
        mov r, eax
    }
    return r;
}

int main(void)
{
    Node n10; Node n30; Node n20; Node headNode; Header hdr;
    n10.pad0=0; n10.pad4=0; n10.right=0; n10.left=0; n10.key=10;
    n30.pad0=0; n30.pad4=0; n30.right=0; n30.left=0; n30.key=30;
    n20.pad0=0; n20.pad4=0; n20.right=&n30; n20.left=&n10; n20.key=20;
    headNode.pad0=0; headNode.pad4=&n20; headNode.right=0; headNode.left=0; headNode.key=0;
    hdr.head=&headNode;

    int t20=20; Node* r20=(Node*)callit(&hdr,&t20);
    if (r20 != &n30) { printf("FAIL t20 %p\n",(void*)r20); return 1; }

    int t30=30; Node* r30=(Node*)callit(&hdr,&t30);
    if (r30 != &headNode) { printf("FAIL t30 %p\n",(void*)r30); return 1; }

    int t5=5; Node* r5=(Node*)callit(&hdr,&t5);
    if (r5 != &n30) { printf("FAIL t5 %p\n",(void*)r5); return 1; }

    printf("OK_0x005b1dd9 t20=n30 t30=head t5=n30\n");
    return 0;
}