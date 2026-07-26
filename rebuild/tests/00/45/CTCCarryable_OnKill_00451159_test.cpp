#include <cstdio>

struct ListHead {
    void* pad0;
    void* next;
    void* prev8;
    void* prev12;
};

static void* g_freed;
void __stdcall FreeNode(void* p) { g_freed = p; }

struct CTCCarryable {
    ListHead* head;
    int flag;
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

int main() {
    ListHead node;
    node.pad0 = 0;
    node.next = (void*)0xDEAD;
    node.prev8 = 0;
    node.prev12 = 0;
    CTCCarryable c;
    c.head = &node;
    c.flag = 1;
    g_freed = 0;
    CTCCarryable_OnKill(&c);
    if (g_freed != (void*)0xDEAD) { std::printf("FAIL free\n"); return 1; }
    if (node.prev8 != &node) { std::printf("FAIL prev8\n"); return 1; }
    if (node.next != 0) { std::printf("FAIL next\n"); return 1; }
    if (node.prev12 != &node) { std::printf("FAIL prev12\n"); return 1; }
    if (c.flag != 0) { std::printf("FAIL flag\n"); return 1; }

    CTCCarryable c2;
    c2.head = 0;
    c2.flag = 0;
    g_freed = (void*)0x1;
    CTCCarryable_OnKill(&c2);
    if (g_freed != (void*)0x1) { std::printf("FAIL noop\n"); return 1; }

    std::printf("CTCCarryable_00451159_TEST PASS\n");
    return 0;
}