#pragma optimize("s",on)
extern void __stdcall freefn(void* p);

struct Node {
    Node* next;   /* +0x0 */
    void* data;   /* +0x4 */
    Node* a;      /* +0x8 */
    Node* b;      /* +0xc */
};

struct CTCCarryable {
    Node* head;   /* +0x0 */
    Node* count;  /* +0x4 */
};

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        freefn(self->head->data);
        self->head->a = self->head;
        self->head->data = 0;
        self->head->b = self->head;
        self->count = 0;
    }
}