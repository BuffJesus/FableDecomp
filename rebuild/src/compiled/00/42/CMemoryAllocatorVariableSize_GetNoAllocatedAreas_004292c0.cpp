#pragma optimize("s",on)
struct Node {
    Node* pad0;   // +0x0
    Node* left;   // +0x4
    Node* right;  // +0x8
    Node* parent; // +0xc
};

unsigned long CMemoryAllocatorVariableSize_GetNoAllocatedAreas(Node* self)
{
    if (self->parent != 0) { // [eax+0xc]
        self = self->parent;
        while (self->right != 0) // [eax+8]
            self = self->right;
        return (unsigned long)self;
    }
    Node* n;
    n = self->left; // [eax+4]
    while (self == n->parent) { // eax == [ecx+0xc]
        self = n;
        n = n->left; // [ecx+4]
    }
    if (self->parent != n) // [eax+0xc] == ecx ? else eax=ecx
        self = n;
    return (unsigned long)self;
}