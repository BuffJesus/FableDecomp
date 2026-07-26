#pragma optimize("gs",on)
struct Node {
    Node* self0;
    unsigned f4;
    Node* f8;
    Node* fc;
};

struct CTCCarryable {
    Node* node;
    unsigned f4;
};

extern void __stdcall ReleaseThing(unsigned arg);

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->f4 != 0) {
        Node* n = self->node;
        ReleaseThing(n->f4);
        Node* p = self->node;
        p->f8 = p;
        Node* q = self->node;
        q->f4 = 0;
        Node* r = self->node;
        r->fc = r;
        self->f4 = 0;
    }
}