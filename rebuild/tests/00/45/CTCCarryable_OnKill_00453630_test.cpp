#include <cstdio>
struct Node { void* p0; void* p4; void* p8; void* pc; };
struct CTCCarryable { Node* node; int flag; };
static void* g_freed = (void*)0;
static int g_called = 0;
extern "C" void __stdcall FreeThing(void* p){ g_freed = p; g_called++; }
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->flag != 0)
    {
        FreeThing(self->node->p4);
        self->node->p8 = self->node;
        self->node->p4 = 0;
        self->node->pc = self->node;
        self->flag = 0;
    }
}
int main(){
    Node n; n.p0=(void*)0; n.p4=(void*)0x1234; n.p8=(void*)0; n.pc=(void*)0;
    CTCCarryable c; c.node=&n; c.flag=1;
    CTCCarryable_OnKill(&c);
    if (g_called!=1){ std::printf("FAIL call\n"); return 1; }
    if (g_freed!=(void*)0x1234){ std::printf("FAIL freed\n"); return 1; }
    if (n.p8!=&n || n.pc!=&n){ std::printf("FAIL selfptr\n"); return 1; }
    if (n.p4!=(void*)0){ std::printf("FAIL p4\n"); return 1; }
    if (c.flag!=0){ std::printf("FAIL flag\n"); return 1; }
    if (c.node!=&n){ std::printf("FAIL node\n"); return 1; }
    CTCCarryable c2; c2.node=(Node*)0; c2.flag=0;
    CTCCarryable_OnKill(&c2);
    if (g_called!=1){ std::printf("FAIL nullskip\n"); return 1; }
    std::printf("CTCCarryable_00453630_TEST PASS\n");
    return 0;
}