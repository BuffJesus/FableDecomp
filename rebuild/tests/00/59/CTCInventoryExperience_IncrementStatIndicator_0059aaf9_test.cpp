
#include <cstdio>

struct SegIter {
    int   *cur;
    int   *first;
    int   *last;
    int  **node;
};

void __fastcall IncrementStatIndicator(SegIter *self)
{
    self->cur = self->cur + 1;
    if (self->cur == self->last) {
        self->node = self->node + 1;
        int *base = *self->node;
        self->first = base;
        self->last  = (int *)((char *)base + 0x80);
        self->cur   = self->first;
    }
}

int main() {
    // block0: 32 ints; block1: 32 ints. node array points at each block base.
    static int block0[32];
    static int block1[32];
    for (int i = 0; i < 32; ++i) { block0[i] = 1000 + i; block1[i] = 2000 + i; }
    static int *nodes[2] = { block0, block1 };

    // --- Case A: mid-block increment (no rollover) ---
    SegIter it;
    it.cur   = &block0[5];
    it.first = &block0[0];
    it.last  = &block0[32];   // first + 0x80 bytes (32 ints)
    it.node  = &nodes[0];

    IncrementStatIndicator(&it);
    if (it.cur != &block0[6]) { printf("FAIL A cur\n"); return 1; }
    if (it.first != &block0[0] || it.last != &block0[32]) { printf("FAIL A block\n"); return 1; }
    if (it.node != &nodes[0]) { printf("FAIL A node\n"); return 1; }

    // --- Case B: rollover to next block ---
    SegIter it2;
    it2.cur   = &block0[31];
    it2.first = &block0[0];
    it2.last  = &block0[32];
    it2.node  = &nodes[0];

    IncrementStatIndicator(&it2);
    // cur advanced to block0[32] == last -> rollover
    if (it2.node != &nodes[1]) { printf("FAIL B node\n"); return 1; }
    if (it2.first != &block1[0]) { printf("FAIL B first\n"); return 1; }
    if (it2.last != (int*)((char*)&block1[0] + 0x80)) { printf("FAIL B last\n"); return 1; }
    if (it2.cur != &block1[0]) { printf("FAIL B cur\n"); return 1; }

    printf("OK_0x0059aaf9\n");
    return 0;
}