
#include <cstdio>

struct Node {
    long pad0;
    long pad4;
    Node* left;
    Node* right;
    unsigned long key;
};
struct Container {
    Node* header;
    Node* root;
};
struct Owner {
    Container* c;
};

__declspec(naked) void FindNodeLinear()
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
        jmp     test_cur
    go_right:
        mov     ecx, dword ptr [ecx+0xc]
    test_cur:
        test    ecx, ecx
        jne     loop_top
    done:
        ret     4
    }
}

static Node* call_find(Owner* self, unsigned long* pkey)
{
    Node* r;
    __asm {
        mov eax, pkey
        push eax
        mov ecx, self
        call FindNodeLinear
        mov r, eax
    }
    return r;
}

int main()
{
    static Node n10 = {0,0,0,0,10};
    static Node n30 = {0,0,0,0,30};
    static Node n20 = {0,0,&n10,&n30,20};
    static Container c;
    c.header = 0;
    c.root = &n20;
    static Owner o;
    o.c = &c;

    unsigned long k15 = 15;
    Node* r1 = call_find(&o, &k15);
    if (r1 != &n20) { printf("FAIL r1\n"); return 1; }

    unsigned long k25 = 25;
    Node* r2 = call_find(&o, &k25);
    if (r2 != &n30) { printf("FAIL r2\n"); return 1; }

    unsigned long k10 = 10;
    Node* r3 = call_find(&o, &k10);
    if (r3 != &n10) { printf("FAIL r3\n"); return 1; }

    unsigned long k99 = 99;
    Node* r4 = call_find(&o, &k99);
    if (r4 != (Node*)&c) { printf("FAIL r4\n"); return 1; }

    printf("OK_0x005d84f2\n");
    return 0;
}