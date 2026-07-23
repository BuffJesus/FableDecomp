// Lift target module #3: CMemoryAllocatorFixedSize (retail Fable.exe).
// Stress-tests the harness on LOOPS (free-list walks), not just flat accessors.
// The allocator keeps a singly-linked free/area list at this+0xc1c; each node is
// { +0: ?, +4: int size/count, +8: node* next }. Scalar members live at 0xc28..0xc44.
//
// GetFragmentation @0x00a66550 is intentionally OMITTED: it is `fld [0x0122dedc]`
// (returns an absolute-addressed global) — its operand is a baked image address, so
// it can only match after a full-image link, never in a standalone TU. That is a
// real class of "link-time-only" functions, documented not dodged.
//
// Build+diff: bash lift/build_module_diff.sh lift/proofs/src/cmemalloc.cpp lift/proofs/oracles/cmemalloc.expected.tsv
//
// Retail decomp + bytes (DumpFuncBytesAndC @ the addresses shown):
//  GetAllocationSize              @00a65d40 (9)  return *(u32*)(this+0xc28)   [unused void* arg -> ret 4]
//     8b 81 28 0c 00 00 c2 04 00
//  GetAllocationSizeIncludingHeader@00a65de0 (9) return *(u32*)(this+0xc2c)   [ret 4]
//     8b 81 2c 0c 00 00 c2 04 00
//  GetNoAllocatedAreas            @00a65e50 (29) sum node.size over the list
//     8b 89 1c 0c 00 00 33 c0 85 c9 74 10 8d 64 24 00 8b 51 04 8b 49 08 03 c2 85 c9 75 f4 c3
//  GetNoFreeAreas                 @00a65e20 (39) sum (areaCap - node.size)
//     8b 91 1c 0c 00 00 33 c0 85 d2 74 1a 8b 89 34 0c 00 00 56 57 8b 7a 04 8b 52 08 8b f1 2b f7 03 c6 85 d2 75 f0 5f 5e c3
//  GetTotalMemoryControlledByAllocator @00a65d50 (32) baseMem + blockSize per node
//     8b 91 1c 0c 00 00 85 d2 8b 81 44 0c 00 00 74 0f 8b 89 38 0c 00 00 8b 52 08 03 c1 85 d2 75 f7 c3
//  GetHeaderOverhead              @00a65d70 (36) sum (hdrPer + node.size*4)
//     8b 91 1c 0c 00 00 33 c0 85 d2 74 17 8b 89 30 0c 00 00 56 8b 72 04 8b 52 08 8d 34 b1 03 c6 85 d2 75 f1 5e c3
//  GetLargestFreeAreaSize         @00a65df0 (48) first node with size!=areaCap -> allocSize else 0
//     8b 81 1c 0c 00 00 85 c0 56 57 74 16 8b 91 34 0c 00 00 8b 78 04 8b f2 2b f7 75 0c 8b 40 08 85 c0 75 f0 5f 33 c0 5e c3 8b 81 28 0c 00 00 5f 5e c3

struct Area { int _pad0; int size; Area* next; };

class CMemoryAllocatorFixedSize {
    unsigned char m_[0xc48];
    Area*  head()      const { return *reinterpret_cast<Area* const*>(m_ + 0xc1c); }
    unsigned u(unsigned off) const { return *reinterpret_cast<const unsigned*>(m_ + off); }
public:
    unsigned GetAllocationSize(void* p) const;
    unsigned GetAllocationSizeIncludingHeader(void* p) const;
    unsigned GetNoAllocatedAreas() const;
    unsigned GetNoFreeAreas() const;
    unsigned GetTotalMemoryControlledByAllocator() const;
    unsigned GetHeaderOverhead() const;
    unsigned GetLargestFreeAreaSize() const;
};

unsigned CMemoryAllocatorFixedSize::GetAllocationSize(void*) const {
    return u(0xc28);
}
unsigned CMemoryAllocatorFixedSize::GetAllocationSizeIncludingHeader(void*) const {
    return u(0xc2c);
}
unsigned CMemoryAllocatorFixedSize::GetNoAllocatedAreas() const {
    unsigned total = 0;
    for (Area* a = head(); a != 0; a = a->next)
        total += a->size;
    return total;
}
unsigned CMemoryAllocatorFixedSize::GetNoFreeAreas() const {
    // Match: retail hoists areaCap into a reg before the loop and loads size by VALUE.
    unsigned total = 0;
    Area* a = head();
    if (a) {
        unsigned cap = u(0xc34);
        do {
            int sz = a->size;
            a = a->next;
            total += cap - sz;
        } while (a);
    }
    return total;
}
unsigned CMemoryAllocatorFixedSize::GetTotalMemoryControlledByAllocator() const {
    unsigned total = u(0xc44);
    for (Area* a = head(); a != 0; a = a->next)
        total += u(0xc38);
    return total;
}
unsigned CMemoryAllocatorFixedSize::GetHeaderOverhead() const {
    // Match: hoist hdrPer into a reg; `hdr + size*4` folds to lea reg,[hdr+size*4].
    unsigned total = 0;
    Area* a = head();
    if (a) {
        unsigned hdr = u(0xc30);
        do {
            int sz = a->size;
            a = a->next;
            total += hdr + sz * 4;
        } while (a);
    }
    return total;
}
unsigned CMemoryAllocatorFixedSize::GetLargestFreeAreaSize() const {
    // Match: hoist areaCap; test via subtraction (cap - size) != 0, not cmp.
    Area* a = head();
    if (a) {
        unsigned cap = u(0xc34);
        do {
            if (cap - (unsigned)a->size != 0)
                return u(0xc28);
            a = a->next;
        } while (a);
    }
    return 0;
}
