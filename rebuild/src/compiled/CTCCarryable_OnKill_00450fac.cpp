#pragma optimize("s",on)
extern void __stdcall FreeNode(void* p);
struct ListNode { ListNode* nextish; void* freeptr; ListNode* nxt; ListNode* prv; };
struct CTCCarryable { ListNode* head; int count; };
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        FreeNode(self->head->freeptr);
        self->head->nxt = self->head;
        self->head->freeptr = 0;
        self->head->prv = self->head;
        self->count = 0;
    }
}