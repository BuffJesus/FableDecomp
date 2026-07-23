#pragma optimize("s",on)
struct Node {
    void* f0;
    void* f4;
    Node* f8;
    Node* fc;
};
struct CTCCarryable {
    Node* head;
    int count;
};
extern void __stdcall freeNode(void* p);

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        Node* n = self->head;
        freeNode(n->f4);
        n = self->head;
        n->f8 = n;
        self->head->f4 = 0;
        n = self->head;
        n->fc = n;
        self->count = 0;
    }
}