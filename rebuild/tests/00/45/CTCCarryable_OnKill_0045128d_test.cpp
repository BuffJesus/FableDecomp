#include <cstdio>

struct Node {
    void* f0;
    void* f4;
    void* f8;
    void* fc;
};

struct CTCCarryable {
    Node* node;
    int   active;
};

static void* g_freed_arg = 0;
static int   g_freed_called = 0;
void __stdcall FreeCarry(void* p) { g_freed_arg = p; g_freed_called++; }

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->active != 0) {
        Node* n = self->node;
        FreeCarry(n->f4);
        n = self->node;
        n->f8 = n;
        n = self->node;
        n->f4 = 0;
        n = self->node;
        n->fc = n;
        self->active = 0;
    }
}

int main() {
    Node n;
    n.f0 = (void*)0x1111;
    n.f4 = (void*)0x2222;
    n.f8 = (void*)0x3333;
    n.fc = (void*)0x4444;
    CTCCarryable c;
    c.node = &n;
    c.active = 1;

    CTCCarryable_OnKill(&c);

    if (g_freed_called != 1) { std::printf("FAIL free count\n"); return 1; }
    if (g_freed_arg != (void*)0x2222) { std::printf("FAIL free arg\n"); return 1; }
    if (n.f8 != (void*)&n) { std::printf("FAIL f8\n"); return 1; }
    if (n.f4 != 0) { std::printf("FAIL f4\n"); return 1; }
    if (n.fc != (void*)&n) { std::printf("FAIL fc\n"); return 1; }
    if (c.active != 0) { std::printf("FAIL active\n"); return 1; }

    // inactive path: nothing happens
    CTCCarryable c2;
    Node n2; n2.f4 = (void*)0x9;
    c2.node = &n2; c2.active = 0;
    g_freed_called = 0;
    CTCCarryable_OnKill(&c2);
    if (g_freed_called != 0) { std::printf("FAIL inactive\n"); return 1; }

    std::printf("CTCCarryable_0045128d_TEST PASS\n");
    return 0;
}