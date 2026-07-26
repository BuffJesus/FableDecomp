#include <cstdio>

struct Node {
    Node* next;
    void* data;
    Node* a;
    Node* b;
};

struct CTCCarryable {
    Node* head;
    Node* count;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;
void __stdcall freefn(void* p) { g_freed++; g_freed_ptr = p; }

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

int main()
{
    Node n;
    n.next = (Node*)0xdead;
    n.data = (void*)0xbeef;
    n.a = 0;
    n.b = 0;
    CTCCarryable c;
    c.head = &n;
    c.count = (Node*)3;

    CTCCarryable_OnKill(&c);

    if (g_freed == 1 && g_freed_ptr == (void*)0xbeef &&
        n.data == 0 && n.a == &n && n.b == &n && c.count == 0) {
        Node emptyNode;
        emptyNode.data = (void*)1;
        CTCCarryable e;
        e.head = &emptyNode;
        e.count = 0;
        int before = g_freed;
        CTCCarryable_OnKill(&e);
        if (before == g_freed) {
            std::printf("CTCCarryable_004512da_TEST PASS\n");
            return 0;
        }
    }

    std::printf("FAIL\n");
    return 1;
}