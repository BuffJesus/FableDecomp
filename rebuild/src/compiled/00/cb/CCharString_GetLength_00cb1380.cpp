struct Node {
    Node* f0;   // +0 (unused by this fn)
    Node* f4;   // +4  next
    long  f8;   // +8  value
};

struct CCharString {
    Node* f0;   // +0
};

long __stdcall GetLength(CCharString* self)
{
    Node* n = self->f0;
    if (n == 0)
        return 0;
    self->f0 = n->f4;
    return n->f8;
}