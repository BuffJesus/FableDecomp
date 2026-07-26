#include <cstdio>

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

static int freed = 0;
static void* freed_ptr = 0;
void __stdcall freeMem(void* p) { freed = 1; freed_ptr = p; }

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

int main()
{
    List L;
    L.head = (void*)0x1111;
    L.alloc = (void*)0x2222;
    L.self1 = (void*)0x3333;
    L.self2 = (void*)0x4444;
    CTCCarryable c;
    c.list = &L;
    c.count = 5;

    CTCCarryable_OnKill(&c);
    if (!(freed == 1 && freed_ptr == (void*)0x2222 &&
          L.self1 == &L && L.alloc == 0 && L.self2 == &L && c.count == 0)) {
        std::printf("FAIL active path\n"); return 1;
    }

    freed = 0; freed_ptr = 0;
    List L2; L2.head=(void*)1; L2.alloc=(void*)2; L2.self1=(void*)3; L2.self2=(void*)4;
    CTCCarryable c2; c2.list=&L2; c2.count=0;
    CTCCarryable_OnKill(&c2);
    if (freed != 0 || c2.count != 0 || L2.alloc != (void*)2) {
        std::printf("FAIL inactive path\n"); return 1;
    }

    std::printf("CTCCarryable_004510bf_TEST PASS\n");
    return 0;
}