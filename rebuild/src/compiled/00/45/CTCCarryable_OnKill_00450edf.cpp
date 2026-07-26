#pragma optimize("s",on)
struct Node { void* f0; long f4; Node* f8; Node* fc; };
struct CTCCarryable { Node* head; long count; };
extern void __stdcall FreeNode(void* p);
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        FreeNode((void*)self->head->f4);
        self->head->f8 = (Node*)self->head;
        self->head->f4 = 0;
        self->head->fc = (Node*)self->head;
        self->count = 0;
    }
}