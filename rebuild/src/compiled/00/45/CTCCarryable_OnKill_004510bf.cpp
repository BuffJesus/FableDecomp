#pragma optimize("s",on)
struct List {
    void* head;
    void* alloc;
    void* self1;
    void* self2;
};
struct CTCCarryable {
    List* list;
    int   count;
};

void __stdcall freeMem(void* p);

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        freeMem(self->list->alloc);
        self->list->self1 = self->list;
        self->list->alloc = 0;
        self->list->self2 = self->list;
        self->count = 0;
    }
}