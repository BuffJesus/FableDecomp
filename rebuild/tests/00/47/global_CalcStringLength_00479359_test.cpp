#include <stdio.h>
struct Node { Node* next; };
struct List {
    Node* head;
    int CalcStringLength();
};
int List::CalcStringLength() {
    Node* sentinel = head;
    Node* p = sentinel->next;
    int n = 0;
    while (p != sentinel) {
        p = p->next;
        ++n;
    }
    return n;
}
int main() {
    // build circular list with sentinel + 3 real nodes
    Node sentinel, a, b, c;
    sentinel.next = &a; a.next = &b; b.next = &c; c.next = &sentinel;
    List lst; lst.head = &sentinel;
    int got = lst.CalcStringLength();
    if (got != 3) { printf("BAD got=%d\n", got); return 1; }

    // empty (sentinel points to itself)
    Node s2; s2.next = &s2;
    List lst2; lst2.head = &s2;
    if (lst2.CalcStringLength() != 0) { printf("BAD empty\n"); return 1; }

    printf("OK_0x00479359 len=%d\n", got);
    return 0;
}