// std::_Find over a circular list: walk from `first` until the node's value matches or
// the range ends, then store the resulting iterator through the sret pointer.
// __fastcall result=ecx, value=edx, (first, last, alloc) on the stack (ret 0xc).
struct Node { Node* next; Node* prev; void* value; };
struct Iter { Node* node; };
extern "C" void __fastcall FindNode(Iter* result, void** value, Node* first, Node* last, int unused) {
    Node* n = first;
    if (n != last) {
        void* v = *value;
        while (n->value != v) {
            n = n->next;
            if (n == last) break;
        }
    }
    result->node = n;
}
