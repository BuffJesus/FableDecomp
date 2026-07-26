#pragma optimize("s",on)
struct Node {
    void* f0;
    void* f4;
    void* f8;
    void* fc;
};

struct CTCCarryable {
    Node* node;   // +0
    int   active; // +4
};

extern void __stdcall FreeCarry(void* p);

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->active != 0) {
        Node* n = self->node;
        FreeCarry(n->f4);
        n = self->node;
        n->f8 = n;
        n = self->node;
        n->f4 = 0;
        n = self->node;
        n->fc = n;
        self->active = 0;
    }
}