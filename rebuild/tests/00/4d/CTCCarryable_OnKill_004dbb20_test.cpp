#include <cstdio>
struct AutoTinyListNode
{
    void* reserved;
    void* allocation;
    AutoTinyListNode* next;
    AutoTinyListNode* previous;
};
struct AutoTinyOwnedList
{
    AutoTinyListNode* head;
    long count;
};
static int g_AutoTinyFreeListCalls = 0;
static void* g_AutoTinyFreedAllocation = 0;
void __stdcall AutoTinyFreeListAllocation(void* allocation)
{
    ++g_AutoTinyFreeListCalls;
    g_AutoTinyFreedAllocation = allocation;
}
void __fastcall OnKill(AutoTinyOwnedList* self)
{
    if (self->count != 0)
    {
        AutoTinyFreeListAllocation(self->head->allocation);
        self->head->next = self->head;
        self->head->allocation = 0;
        self->head->previous = self->head;
        self->count = 0;
    }
}
int main()
{
    int allocation = 0;
    AutoTinyListNode head = {0, &allocation, 0, 0};
    AutoTinyOwnedList list = {&head, 1};
    OnKill(&list);
    if (g_AutoTinyFreeListCalls != 1 ||
        g_AutoTinyFreedAllocation != &allocation ||
        head.next != &head || head.previous != &head ||
        head.allocation != 0 || list.count != 0)
        return 1;
    OnKill(&list);
    if (g_AutoTinyFreeListCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dbb20_TEST PASS\n");
    return 0;
}