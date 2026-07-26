#include <cstdio>
struct Node {
    void* f0;
    void* f4;
    Node* f8;
    Node* fc;
};
struct CTCCarryable {
    Node* node;
    int flag;
};
static void* g_released = (void*)0;
void __stdcall ReleaseThing(void* p) { g_released = p; }
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->flag != 0) {
        Node* n = self->node;
        ReleaseThing(n->f4);
        n = self->node;
        n->f8 = n;
        n = self->node;
        n->f4 = 0;
        n = self->node;
        n->fc = n;
        self->flag = 0;
    }
}
int main()
{
    Node n;
    n.f0 = (void*)0x1111;
    n.f4 = (void*)0x2222;
    n.f8 = (Node*)0;
    n.fc = (Node*)0;
    CTCCarryable c;
    c.node = &n;
    c.flag = 1;
    CTCCarryable_OnKill(&c);
    if (g_released == (void*)0x2222 && n.f8 == &n && n.f4 == (void*)0 && n.fc == &n && c.flag == 0) {
        std::printf("CTCCarryable_00451240_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}