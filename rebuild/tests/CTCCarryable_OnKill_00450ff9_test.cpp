#include <cstdio>
struct Node { void* p0; void* f4; Node* f8; Node* fc; };
struct CTCCarryable { Node* head; long count; };
static void* g_freed;
void __stdcall FreeChunk(void* p){ g_freed = p; }
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        FreeChunk(self->head->f4);
        self->head->f8 = self->head;
        self->head->f4 = 0;
        self->head->fc = self->head;
        self->count = 0;
    }
}
int main(){
    Node n; n.p0=0; n.f4=(void*)0x1234; n.f8=0; n.fc=0;
    CTCCarryable c; c.head=&n; c.count=5;
    CTCCarryable_OnKill(&c);
    if (g_freed==(void*)0x1234 && n.f8==&n && n.f4==0 && n.fc==&n && c.count==0) {
        std::printf("CTCCarryable_00450ff9_TEST PASS\n"); return 0;
    }
    std::printf("FAIL\n"); return 1;
}