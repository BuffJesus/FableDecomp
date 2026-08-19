#pragma optimize("s",on)
// Circular-list length (manifest name `CalcStringLength`): walk from head->next back
// to the sentinel. __fastcall this=ecx, no args.
struct Node { Node* next; };
struct List { Node* head; int Count() const; };
int List::Count() const {
    Node* h = this->head;
    Node* n = h->next;
    int count = 0;
    while (n != h) { n = n->next; ++count; }
    return count;
}
