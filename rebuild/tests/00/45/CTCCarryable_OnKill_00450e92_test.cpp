#include <cstdio>
struct Node {
    void* f0;
    void* f4;
    Node* f8;
    Node* fc;
};
struct CTCCarryable {
    Node* head;
    int count;
};
static void* g_freed = (void*)0;
void __stdcall freeNode(void* p) { g_freed = p; }

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        Node* n = self->head;
        freeNode(n->f4);
        n = self->head;
        n->f8 = n;
        self->head->f4 = 0;
        n = self->head;
        n->fc = n;
        self->count = 0;
    }
}

int main() {
    Node node;
    node.f0 = (void*)0x1111;
    node.f4 = (void*)0x2222;
    node.f8 = (Node*)0x3333;
    node.fc = (Node*)0x4444;
    CTCCarryable c;
    c.head = &node;
    c.count = 5;
    CTCCarryable_OnKill(&c);
    if (g_freed == (void*)0x2222 && node.f8 == &node && node.f4 == 0 && node.fc == &node && c.count == 0) {
        std::printf("CTCCarryable_00450e92_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL g=%p f8=%p f4=%p fc=%p cnt=%d\n", g_freed, node.f8, node.f4, node.fc, c.count);
    return 1;
}