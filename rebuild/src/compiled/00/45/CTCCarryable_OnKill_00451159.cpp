#pragma optimize("s",on)
struct ListHead {
    void* pad0;    /* +0 */
    void* next;    /* +4 */
    void* prev8;   /* +8 */
    void* prev12;  /* +0xc */
};

extern void __stdcall FreeNode(void* p);

struct CTCCarryable {
    ListHead* head;  /* +0 */
    int flag;        /* +4 */
};

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->flag != 0) {
        ListHead* h = self->head;
        FreeNode(h->next);
        h = self->head;
        h->prev8 = h;
        h = self->head;
        h->next = 0;
        h = self->head;
        h->prev12 = h;
        self->flag = 0;
    }
}