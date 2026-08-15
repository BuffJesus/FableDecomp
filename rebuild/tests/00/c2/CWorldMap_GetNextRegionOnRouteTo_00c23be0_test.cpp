#include <cstdio>

struct CWorldMap {
    char pad0[8];
    long count;
    char pad2[8];
    long* array;
};

static long g_helper_ret = 0;
static long CWorldMap_helper(CWorldMap* self) { (void)self; return g_helper_ret; }

static long GetNextRegionOnRouteTo(CWorldMap* self)
{
    if (self->count == 0) return 0;
    long idx = CWorldMap_helper(self);
    long r = self->array[idx];
    self->count = self->count - 1;
    return r;
}

int main() {
    long arr[4] = {100, 200, 300, 400};
    CWorldMap m; m.count = 3; m.array = arr;

    CWorldMap e; e.count = 0; e.array = arr;
    if (GetNextRegionOnRouteTo(&e) != 0) { printf("FAIL empty\n"); return 1; }

    g_helper_ret = 2;
    if (GetNextRegionOnRouteTo(&m) != 300) { printf("FAIL idx\n"); return 1; }
    if (m.count != 2) { printf("FAIL dec\n"); return 1; }

    g_helper_ret = 0;
    if (GetNextRegionOnRouteTo(&m) != 100) { printf("FAIL idx2\n"); return 1; }
    if (m.count != 1) { printf("FAIL dec2\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}