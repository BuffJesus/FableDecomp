#include <cstdio>
struct Node { void* f0; long f4; struct Node* f8; struct Node* fc; };
struct CTCCarryable { Node* head; long count; };
static void* g_freed = (void*)1;
void __stdcall FreeNode(void* p){ g_freed = p; }
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        FreeNode((void*)self->head->f4);
        self->head->f8 = (Node*)self->head;
        self->head->f4 = 0;
        self->head->fc = (Node*)self->head;
        self->count = 0;
    }
}
int main(){
    Node n; n.f0=0; n.f4=(long)0xABCD; n.f8=0; n.fc=0;
    CTCCarryable c; c.head=&n; c.count=5;
    CTCCarryable_OnKill(&c);
    if (g_freed != (void*)0xABCD) { std::printf("FAIL freed\n"); return 1; }
    if (n.f8 != &n || n.fc != &n || n.f4 != 0 || c.count != 0) { std::printf("FAIL state\n"); return 1; }
    /* also verify no-free path */
    Node m; m.f0=0; m.f4=(long)0x1234; m.f8=0; m.fc=0;
    CTCCarryable d; d.head=&m; d.count=0; g_freed=(void*)7;
    CTCCarryable_OnKill(&d);
    if (g_freed != (void*)7) { std::printf("FAIL nofree\n"); return 1; }
    if (m.f8 != 0 || m.fc != 0 || m.f4 != (long)0x1234 || d.count != 0) { std::printf("FAIL state2\n"); return 1; }
    std::printf("CTCCarryable_00450edf_TEST PASS\n");
    return 0;
}