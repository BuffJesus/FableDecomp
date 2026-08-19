// Copy the node out of the held impl into the caller's iterator.
// __fastcall this=ecx, out=stack (ret 4).
struct Node { Node* next; };
struct Impl { Node* node; };
struct Iter { Impl* impl; void Begin(Impl* out); };
void Iter::Begin(Impl* out) {
    Node* n = this->impl->node;
    out->node = n;
}
