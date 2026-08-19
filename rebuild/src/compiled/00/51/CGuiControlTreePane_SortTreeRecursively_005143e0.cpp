
struct Node { Node* next; };
struct Tree { Node* head; };
int __fastcall SortTreeRecursively(Tree* self) {
    Node* sentinel = self->head;
    Node* p = sentinel->next;
    int count = 0;
    while (p != sentinel) { p = p->next; ++count; }
    return count;
}