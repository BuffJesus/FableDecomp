#include <cstdio>

// Behaviour model: _Destroy_range steps by element size 0x14 from first to last,
// calling the per-element destroy on each, in order. Verify it visits exactly the
// right elements (count and order), stepping by 20 bytes.

struct Pair { unsigned long key; char value[0x10]; };

static int g_visited[64];
static int g_n = 0;

static void destroy_one(Pair* p) { g_visited[g_n++] = (int)p->key; }

static void destroy_range(Pair* first, Pair* last) {
    for (; first != last; ++first)
        destroy_one(first);
}

int main() {
    Pair arr[5];
    for (int i = 0; i < 5; ++i) arr[i].key = (unsigned long)(100 + i);

    // sizeof(Pair) must be 0x14
    if (sizeof(Pair) != 0x14) { printf("BADSIZE\n"); return 1; }

    destroy_range(&arr[0], &arr[5]);

    if (g_n != 5) { printf("BADCOUNT\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (g_visited[i] != 100 + i) { printf("BADORDER\n"); return 1; }

    // empty range visits nothing
    g_n = 0;
    destroy_range(&arr[0], &arr[0]);
    if (g_n != 0) { printf("BADEMPTY\n"); return 1; }

    printf("DESTROY_RANGE_OK\n");
    return 0;
}