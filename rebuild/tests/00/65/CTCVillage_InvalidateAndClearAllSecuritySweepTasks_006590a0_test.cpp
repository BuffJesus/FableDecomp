#include <cstdio>

struct Task { char pad[0x30]; unsigned char flag; };
struct ListNode { ListNode* next; ListNode* prev; Task* task; };
struct List { ListNode* head; };
struct CTCVillage { char pad[0x5c]; List list; };

static int g_cleared = 0;
static void __fastcall List_Clear(List* self) { (void)self; g_cleared = 1; }

static void __fastcall Invalidate(CTCVillage* self)
{
    List* lst = &self->list;
    ListNode* sentinel = lst->head;
    ListNode* n = sentinel->next;
    while (n != sentinel) {
        n->task->flag = 1;
        n = n->next;
    }
    List_Clear(lst);
}

int main()
{
    // Build a 3-node circular list with a sentinel.
    ListNode sentinel;
    Task t1, t2, t3;
    ListNode n1, n2, n3;
    t1.flag = 0; t2.flag = 0; t3.flag = 0;
    n1.task = &t1; n2.task = &t2; n3.task = &t3;

    // circular doubly-linked: sentinel -> n1 -> n2 -> n3 -> sentinel
    sentinel.next = &n1; n1.prev = &sentinel;
    n1.next = &n2; n2.prev = &n1;
    n2.next = &n3; n3.prev = &n2;
    n3.next = &sentinel; sentinel.prev = &n3;

    CTCVillage v;
    v.list.head = &sentinel;

    Invalidate(&v);

    if (t1.flag == 1 && t2.flag == 1 && t3.flag == 1 && g_cleared == 1) {
        // also verify empty-list path doesn't touch anything / still clears
        ListNode se2; se2.next = &se2; se2.prev = &se2;
        CTCVillage v2; v2.list.head = &se2;
        g_cleared = 0;
        Invalidate(&v2);
        if (g_cleared == 1) {
            printf("PARITY_OK\n");
            return 0;
        }
    }
    printf("FAIL\n");
    return 1;
}