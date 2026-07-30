
#include <stdio.h>

struct Node { Node* next; };
struct Tree { Node* head; };

int __fastcall SortTreeRecursively(Tree* self) {
    Node* sentinel = self->head;
    Node* p = sentinel->next;
    int count = 0;
    while (p != sentinel) { p = p->next; ++count; }
    return count;
}

int main() {
    // Build a circular singly-linked list with a sentinel node.
    // sentinel -> a -> b -> c -> sentinel
    Node sentinel, a, b, c;
    sentinel.next = &a;
    a.next = &b;
    b.next = &c;
    c.next = &sentinel;
    Tree t; t.head = &sentinel;

    int n = SortTreeRecursively(&t);
    if (n != 3) { printf("FAIL count=%d expected 3\n", n); return 1; }

    // Empty case: sentinel points to itself.
    Node s2; s2.next = &s2;
    Tree t2; t2.head = &s2;
    int m = SortTreeRecursively(&t2);
    if (m != 0) { printf("FAIL empty=%d expected 0\n", m); return 1; }

    printf("OK_0x004b87d0 count=%d empty=%d\n", n, m);
    return 0;
}