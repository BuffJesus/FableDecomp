#include <stdio.h>

struct Node
{
    char pad0[0x20];
    void* m20;
    char pad1[0x14];
    Node* m38;
    void* m3c;
    void* m40;
    void* m44;
};

void __fastcall ClearPrimitiveList(Node* self)
{
    Node* p = self;
    while (p != 0)
    {
        Node* next = p->m38;
        p->m20 = 0;
        p->m38 = 0;
        p->m3c = 0;
        p->m40 = 0;
        p->m44 = 0;
        p = next;
    }
}

int main()
{
    Node a, b, c;
    a.m20 = (void*)1; a.m3c = (void*)1; a.m40 = (void*)1; a.m44 = (void*)1;
    b.m20 = (void*)1; b.m3c = (void*)1; b.m40 = (void*)1; b.m44 = (void*)1;
    c.m20 = (void*)1; c.m3c = (void*)1; c.m40 = (void*)1; c.m44 = (void*)1;
    a.m38 = &b; b.m38 = &c; c.m38 = 0;

    ClearPrimitiveList(&a);

    int ok = 1;
    Node* arr[3]; arr[0]=&a; arr[1]=&b; arr[2]=&c;
    for (int i = 0; i < 3; i++)
    {
        if (arr[i]->m20 || arr[i]->m38 || arr[i]->m3c || arr[i]->m40 || arr[i]->m44)
            ok = 0;
    }

    ClearPrimitiveList(0);

    if (ok) printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}