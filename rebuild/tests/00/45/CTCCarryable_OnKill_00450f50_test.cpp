#include <cstdio>

struct Node {
    void* field0;
    void* next;
    void* field8;
    void* fieldC;
};

struct CTCCarryable {
    Node* obj;
    int   count;
};

static void* g_released = (void*)0xDEADBEEF;
void __stdcall ReleaseThing(void* p) { g_released = p; }

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        ReleaseThing(self->obj->next);
        self->obj->field8 = self->obj;
        self->obj->next = 0;
        self->obj->fieldC = self->obj;
        self->count = 0;
    }
}

int main()
{
    Node n;
    n.field0 = (void*)1;
    n.next   = (void*)0xABCD;
    n.field8 = (void*)2;
    n.fieldC = (void*)3;

    CTCCarryable c;
    c.obj = &n;
    c.count = 5;

    CTCCarryable_OnKill(&c);

    if (g_released != (void*)0xABCD) { std::printf("FAIL release\n"); return 1; }
    if (n.field8 != &n) { std::printf("FAIL field8\n"); return 1; }
    if (n.next != 0) { std::printf("FAIL next\n"); return 1; }
    if (n.fieldC != &n) { std::printf("FAIL fieldC\n"); return 1; }
    if (c.count != 0) { std::printf("FAIL count\n"); return 1; }

    // count==0 path: no-op
    CTCCarryable c2;
    Node n2; n2.next = (void*)0x1111;
    c2.obj = &n2; c2.count = 0;
    g_released = (void*)0x9999;
    CTCCarryable_OnKill(&c2);
    if (g_released != (void*)0x9999) { std::printf("FAIL noop\n"); return 1; }

    std::printf("CTCCarryable_00450f50_TEST PASS\n");
    return 0;
}