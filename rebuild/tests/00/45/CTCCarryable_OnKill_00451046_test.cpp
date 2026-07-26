#include <cstdio>
struct Node { void* p0; void* p4; void* p8; void* pc; };
struct CTCCarryable { Node* node; int slot; };
static int g_freed = 0;
static void* g_freed_arg = 0;
void __stdcall FreeThing(void* p) { g_freed++; g_freed_arg = p; }
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->slot != 0) {
        FreeThing(self->node->p4);
        self->node->p8 = self->node;
        self->node->p4 = 0;
        self->node->pc = self->node;
        self->slot = 0;
    }
}
int main() {
    Node n; n.p0=(void*)1; n.p4=(void*)0x1234; n.p8=(void*)2; n.pc=(void*)3;
    CTCCarryable c; c.node=&n; c.slot=5;
    CTCCarryable_OnKill(&c);
    if (g_freed!=1) { std::printf("FAIL free count %d\n", g_freed); return 1; }
    if (g_freed_arg!=(void*)0x1234) { std::printf("FAIL free arg\n"); return 1; }
    if (n.p8!=&n || n.pc!=&n || n.p4!=0) { std::printf("FAIL node reset\n"); return 1; }
    if (c.slot!=0) { std::printf("FAIL slot\n"); return 1; }
    // slot==0 path: no free
    g_freed=0; c.slot=0; n.p4=(void*)0x99; n.p8=(void*)0x1; n.pc=(void*)0x2;
    CTCCarryable_OnKill(&c);
    if (g_freed!=0) { std::printf("FAIL slot0 freed\n"); return 1; }
    if (c.slot!=0) { std::printf("FAIL slot0\n"); return 1; }
    std::printf("CTCCarryable_00451046_TEST PASS\n");
    return 0;
}