#pragma optimize("s",on)
// InvalidateAndClearAllSecuritySweepTasks 0x006590a0  __fastcall(CTCVillage*)
// this+0x5c is an intrusive doubly-linked list (sentinel node). Each list node:
//   +0 next, +8 payload(task). task+0x30 = invalidate flag byte.
// After marking all tasks invalid, tail-calls the list clear routine.

struct Task { char pad[0x30]; unsigned char flag; };

struct ListNode {
    ListNode* next;   // +0
    ListNode* prev;   // +4
    Task*     task;   // +8
};

struct List {
    ListNode* head;   // +0  -> sentinel
};

struct CTCVillage {
    char pad[0x5c];
    List list;        // +0x5c
};

// The tail-called clear routine (0x65fefc). Thiscall on the List* (ecx = this+0x5c).
extern "C" void __fastcall List_Clear(List* self);

void __fastcall InvalidateAndClearAllSecuritySweepTasks(CTCVillage* self)
{
    List* lst = &self->list;                 // add ecx,0x5c
    ListNode* sentinel = lst->head;          // mov edx,[ecx]
    ListNode* n = sentinel->next;            // mov eax,[edx]
    while (n != sentinel) {                   // cmp/je
        n->task->flag = 1;                    // esi=[eax+8]; byte[esi+0x30]=1
        n = n->next;                          // mov eax,[eax]
    }
    List_Clear(lst);                          // jmp 0x65fefc (tail call, ecx preserved)
}