#include <cstdio>

struct Node {
    Node* self0;
    unsigned f4;
    Node* f8;
    Node* fc;
};

struct CTCCarryable {
    Node* node;
    unsigned f4;
};

static unsigned g_released = 0xdead;
void __stdcall ReleaseThing(unsigned arg) { g_released = arg; }

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->f4 != 0) {
        Node* n = self->node;
        ReleaseThing(n->f4);
        Node* p = self->node;
        p->f8 = p;
        Node* q = self->node;
        q->f4 = 0;
        Node* r = self->node;
        r->fc = r;
        self->f4 = 0;
    }
}

int main() {
    Node nd;
    nd.self0 = (Node*)0x1111;
    nd.f4 = 0x42;
    nd.f8 = (Node*)0x2222;
    nd.fc = (Node*)0x3333;

    CTCCarryable c;
    c.node = &nd;
    c.f4 = 1;

    CTCCarryable_OnKill(&c);

    bool ok = true;
    if (g_released != 0x42) ok = false;
    if (nd.f8 != &nd) ok = false;
    if (nd.f4 != 0) ok = false;
    if (nd.fc != &nd) ok = false;
    if (c.f4 != 0) ok = false;

    // guard branch: f4==0 -> no-op
    g_released = 0xbeef;
    CTCCarryable c2;
    Node nd2; nd2.f4 = 5; nd2.f8 = 0; nd2.fc = 0;
    c2.node = &nd2; c2.f4 = 0;
    CTCCarryable_OnKill(&c2);
    if (g_released != 0xbeef) ok = false;
    if (c2.f4 != 0) ok = false;

    if (ok) { std::printf("CTCCarryable_0045110c_TEST PASS\n"); return 0; }
    std::printf("CTCCarryable_0045110c_TEST FAIL\n"); return 1;
}