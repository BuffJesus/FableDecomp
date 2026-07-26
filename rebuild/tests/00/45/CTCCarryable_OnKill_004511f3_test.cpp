#include <cstdio>
struct Node { Node* self0; unsigned int f4; Node* f8; Node* fc; };
struct CTCCarryable { Node* node; int count; };
static unsigned int g_freed = 0xDEAD;
extern "C" void __stdcall Freer(unsigned int x) { g_freed = x; }
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        Node* n = self->node;
        Freer(n->f4);
        self->node->f8 = self->node;
        self->node->f4 = 0;
        self->node->fc = self->node;
        self->count = 0;
    }
}
int main() {
    Node n; n.self0=&n; n.f4=0x1234; n.f8=0; n.fc=0;
    CTCCarryable c; c.node=&n; c.count=5;
    CTCCarryable_OnKill(&c);
    if (g_freed==0x1234 && n.f8==&n && n.f4==0 && n.fc==&n && c.count==0) {
        std::printf("CTCCarryable_004511f3_TEST PASS\n"); return 0;
    }
    std::printf("FAIL\n"); return 1;
}