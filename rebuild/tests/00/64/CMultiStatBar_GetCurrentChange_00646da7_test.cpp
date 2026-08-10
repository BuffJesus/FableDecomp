#include <stdio.h>

struct Node {
    Node* next;
    long  pad;
    long  pad2;
    long  value;
};

struct CMultiStatBar {
    char  pad0[0x28];
    long  mode;
    char  pad1[0x68 - 0x2c];
    Node* head;
};

long GetCurrentChange_model(CMultiStatBar* self)
{
    if (self->mode != 1)
    {
        Node* h = self->head;
        if (h->next != h)
            return h->next->value;
    }
    return 0;
}

int main()
{
    CMultiStatBar bar;
    Node headSentinel;
    Node item;

    // Case 1: mode == 1 -> returns 0 (regardless of list)
    bar.mode = 1;
    bar.head = &headSentinel;
    headSentinel.next = &item;
    item.value = 42;
    if (GetCurrentChange_model(&bar) != 0) { printf("FAIL mode1\n"); return 1; }

    // Case 2: mode != 1, empty list (head->next == head) -> returns 0
    bar.mode = 0;
    headSentinel.next = &headSentinel;
    if (GetCurrentChange_model(&bar) != 0) { printf("FAIL empty\n"); return 1; }

    // Case 3: mode != 1, non-empty -> returns first item value
    headSentinel.next = &item;
    item.value = 42;
    if (GetCurrentChange_model(&bar) != 42) { printf("FAIL value\n"); return 1; }

    // Case 4: another value (negative)
    item.value = -7;
    if (GetCurrentChange_model(&bar) != -7) { printf("FAIL value2\n"); return 1; }

    // Case 5: mode==1 wins over non-empty list
    bar.mode = 1;
    item.value = 99;
    if (GetCurrentChange_model(&bar) != 0) { printf("FAIL mode1wins\n"); return 1; }

    printf("OK_00646da7\n");
    return 0;
}