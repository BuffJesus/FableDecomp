#include <cstdio>

struct Node {
    Node* pad0;
    Node* left;
    Node* right;
    Node* parent;
};

unsigned long CMemoryAllocatorVariableSize_GetNoAllocatedAreas(Node* self)
{
    if (self->parent != 0) {
        self = self->parent;
        while (self->right != 0)
            self = self->right;
        return (unsigned long)self;
    }
    Node* n;
    n = self->left;
    while (self == n->parent) {
        self = n;
        n = n->left;
    }
    if (self->parent != n)
        self = n;
    return (unsigned long)self;
}

int main()
{
    // Branch 1: parent != 0, walk right until right==0
    Node a, b, c;
    a.pad0 = a.left = a.right = 0; a.parent = &b;
    b.pad0 = b.left = 0; b.parent = 0; b.right = &c;
    c.pad0 = c.left = c.right = c.parent = 0;
    // a.parent = b (!=0). loop: eax=b, ecx=b.right=c(!=0); eax=c, ecx=c.right=0 -> ret c
    unsigned long r1 = CMemoryAllocatorVariableSize_GetNoAllocatedAreas(&a);
    if (r1 != (unsigned long)&c) { std::printf("FAIL branch1 got %p\n", (void*)r1); return 1; }

    // Branch 2: parent == 0 -> leftmost-style traversal
    // Build so that eax==ecx->parent breaks quickly.
    Node h, x;
    h.pad0 = h.right = h.parent = 0; h.left = &x;
    x.pad0 = x.left = x.right = 0; x.parent = &h; // x.parent == h (=eax initially)
    // ecx = h.left = x. check eax(h)==ecx(x).parent(h)? yes -> loop: eax=x, ecx=x.left=0
    //   at L1f: eax(x)==ecx(0).parent -> deref null. Avoid: make x.left point somewhere valid.
    Node y;
    x.left = &y;
    y.pad0 = y.left = y.right = 0; y.parent = 0; // y.parent(0) != x -> break
    // trace: eax=h,ecx=x; eax==x.parent(h)? yes -> eax=x,ecx=x.left=y; eax(x)==y.parent(0)? no -> break
    // then eax->parent(x.parent=h)==ecx(y)? no -> return ecx=y
    unsigned long r2 = CMemoryAllocatorVariableSize_GetNoAllocatedAreas(&h);
    if (r2 != (unsigned long)&y) { std::printf("FAIL branch2 got %p exp %p\n", (void*)r2, (void*)&y); return 1; }

    // Branch 2b: immediate break, return eax when eax->parent==ecx
    Node m, k;
    m.pad0 = m.right = m.parent = 0; m.left = &k;
    k.pad0 = k.left = k.right = 0; k.parent = 0; // k.parent(0) != m -> break immediately
    // ecx=m.left=k; eax(m)==k.parent(0)? no -> break; eax->parent(m.parent=0)==ecx(k)? no -> return k
    unsigned long r3 = CMemoryAllocatorVariableSize_GetNoAllocatedAreas(&m);
    if (r3 != (unsigned long)&k) { std::printf("FAIL branch2b got %p\n", (void*)r3); return 1; }

    std::printf("CMemoryAllocatorVariableSize_004292c0_TEST PASS\n");
    return 0;
}