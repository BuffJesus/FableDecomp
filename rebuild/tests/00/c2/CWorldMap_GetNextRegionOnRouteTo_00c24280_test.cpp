#include <stdio.h>

struct CWorldMap {
    char pad0[8];
    long count;   // +0x8
    char pad2[8];
    long* arr;    // +0x14
};

static long g_idx = 0;
long __fastcall CWorldMap_ComputeIndex(CWorldMap* self) { (void)self; return g_idx; }

long __fastcall GetNextRegionOnRouteTo(CWorldMap* self)
{
    if (self->count == 0)
        return 0;
    long idx = CWorldMap_ComputeIndex(self);
    long result = self->arr[idx];
    self->count = self->count - 1;
    return result;
}

int main()
{
    long data[4] = {100, 200, 300, 400};
    CWorldMap m;
    m.count = 3;
    m.arr = data;

    // empty case
    CWorldMap e;
    e.count = 0;
    e.arr = data;
    if (GetNextRegionOnRouteTo(&e) != 0) { printf("FAIL empty\n"); return 1; }
    if (e.count != 0) { printf("FAIL empty count\n"); return 1; }

    // pick index 2 -> 300, count decrements 3->2
    g_idx = 2;
    long r = GetNextRegionOnRouteTo(&m);
    if (r != 300) { printf("FAIL val %ld\n", r); return 1; }
    if (m.count != 2) { printf("FAIL count %ld\n", m.count); return 1; }

    // pick index 0 -> 100, count 2->1
    g_idx = 0;
    r = GetNextRegionOnRouteTo(&m);
    if (r != 100) { printf("FAIL val2 %ld\n", r); return 1; }
    if (m.count != 1) { printf("FAIL count2 %ld\n", m.count); return 1; }

    printf("BEHAVIOR_OK\n");
    return 0;
}