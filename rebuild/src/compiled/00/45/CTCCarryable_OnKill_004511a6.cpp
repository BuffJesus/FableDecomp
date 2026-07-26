#pragma optimize("s",on)
struct Node {
    Node* v0;     // +0
    int   v4;     // +4
    Node* v8;     // +8
    Node* vc;     // +0xc
};
struct List {
    Node* head;   // +0
    int   node;   // +4
};
extern void __stdcall FreeNode(int p);

void __fastcall CTCCarryable_OnKill(List* self)
{
    if (self->node) {
        Node* n = self->head;
        FreeNode(n->v4);
        self->head->v8 = self->head;
        self->head->v4 &= 0;
        self->head->vc = self->head;
        self->node &= 0;
    }
}