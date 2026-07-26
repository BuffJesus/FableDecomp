struct Node { void* p0; void* p4; void* p8; void* pc; };
struct CTCCarryable { Node* node; int slot; };
void __stdcall FreeThing(void* p);
#pragma optimize("s",on)
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->slot != 0) {
        FreeThing(self->node->p4);
        self->node->p8 = self->node;
        self->node->p4 = 0;
        self->node->pc = self->node;
        self->slot = 0;
    }
}