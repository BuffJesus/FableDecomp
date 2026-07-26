#pragma optimize("s",on)
struct Node { Node* self0; unsigned int f4; Node* f8; Node* fc; };
struct CTCCarryable { Node* node; int count; };
extern "C" void __stdcall Freer(unsigned int x);
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        Node* n = self->node;
        Freer(n->f4);
        self->node->f8 = self->node;
        self->node->f4 = 0;
        self->node->fc = self->node;
        self->count = 0;
    }
}