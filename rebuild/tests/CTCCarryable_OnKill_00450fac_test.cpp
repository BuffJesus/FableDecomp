#include <cstdio>
static int g_freed = 0;
void __stdcall FreeNode(void* p){ (void)p; g_freed++; }
struct ListNode { ListNode* nextish; void* freeptr; ListNode* nxt; ListNode* prv; };
struct CTCCarryable { ListNode* head; int count; };
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        FreeNode(self->head->freeptr);
        self->head->nxt = self->head;
        self->head->freeptr = 0;
        self->head->prv = self->head;
        self->count = 0;
    }
}
int main(){
    ListNode node; node.nextish=(ListNode*)1; node.freeptr=(void*)0x1234; node.nxt=0; node.prv=0;
    CTCCarryable c; c.head=&node; c.count=3;
    CTCCarryable_OnKill(&c);
    if (c.count!=0){ std::printf("FAIL count\n"); return 1; }
    if (g_freed!=1){ std::printf("FAIL freed\n"); return 1; }
    if (node.nxt!=&node || node.prv!=&node || node.freeptr!=0){ std::printf("FAIL node\n"); return 1; }
    CTCCarryable c2; c2.head=&node; c2.count=0;
    int before=g_freed;
    CTCCarryable_OnKill(&c2);
    if (g_freed!=before){ std::printf("FAIL emptypath\n"); return 1; }
    std::printf("CTCCarryable_00450fac_TEST PASS\n");
    return 0;
}