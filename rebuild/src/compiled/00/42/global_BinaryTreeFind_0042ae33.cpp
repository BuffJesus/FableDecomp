#pragma optimize("g",on)
struct Node { int a; Node* left; Node* r8; int k3; int key; };
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