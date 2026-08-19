// Circular doubly-linked list clear (mislabelled `SortTreeRecursively` in the manifest).
// __fastcall this=ecx. this+0 = sentinel node; node[0]=next, node[4]=prev.
// The head re-reads are faithful: the free helper may alias, so the loop test reloads it.
struct Node { Node* next; Node* prev; };
struct List { Node* head; void Clear(); };
extern "C" void __cdecl Free2(void* p);   // 0x00bfea14
void List::Clear() {
    Node* n = this->head->next;
    while (n != this->head) {
        Node* cur = n;
        n = n->next;
        Free2(cur);
    }
    this->head->next = this->head;
    this->head->prev = this->head;
}
