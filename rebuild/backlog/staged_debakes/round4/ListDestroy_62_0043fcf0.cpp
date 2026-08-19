// Circular-list teardown: free every node, re-point the sentinel at itself, then free
// the sentinel. __fastcall this=ecx, no args.
struct Node { Node* next; Node* prev; };
struct List { Node* head; void Destroy(); };
extern "C" void __cdecl Free2(void* p);   // 0x00bfea14
void List::Destroy() {
    Node* n = this->head->next;
    while (n != this->head) {
        Node* cur = n;
        n = n->next;
        Free2(cur);
    }
    this->head->next = this->head;
    this->head->prev = this->head;
    Node* h = this->head;
    if (h) Free2(h);
}
