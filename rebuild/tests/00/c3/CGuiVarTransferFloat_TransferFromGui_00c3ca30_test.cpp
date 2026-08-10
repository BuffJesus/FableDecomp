#include <stdio.h>

struct Node {
    int f0;
    int f4;
    Node* f8;
};
struct CGuiVarTransferFloat {
    int f0;
    int f4;
    Node* f8;
    int fC;
};

static Node* g_called_with = 0;

void __fastcall NodeReturn(Node* n) { g_called_with = n; }

void __fastcall TransferFromGui(CGuiVarTransferFloat* self)
{
    Node* n = self->f8;
    self->f8 = n->f8;
    NodeReturn(n);
    self->fC--;
}

int main()
{
    Node tail; tail.f8 = 0;
    Node head; head.f8 = &tail;
    CGuiVarTransferFloat obj;
    obj.f8 = &head;
    obj.fC = 5;

    TransferFromGui(&obj);

    int ok = 1;
    if (obj.f8 != &tail) ok = 0;         // list advanced past head
    if (g_called_with != &head) ok = 0;  // called on old head node
    if (obj.fC != 4) ok = 0;             // counter decremented

    // second pop
    TransferFromGui(&obj);
    if (obj.f8 != 0) ok = 0;
    if (g_called_with != &tail) ok = 0;
    if (obj.fC != 3) ok = 0;

    if (ok) printf("OK_00c3ca30\n");
    else    printf("FAIL\n");
    return ok ? 0 : 1;
}