#include <stdio.h>

struct AllocT { int tag; };

struct Pair {
    unsigned long first;
    void* second;
};

static int g_calls = 0;
static Pair* g_ptrs[16];
static AllocT* g_al = 0;

void destroy_one(Pair* elem, AllocT* al) {
    g_ptrs[g_calls] = elem;
    g_al = al;
    g_calls++;
}

// behaviour model of Destroy_range: iterate [first,last) stride 8, call destroy on each
void Destroy_range(Pair* first, Pair* last, AllocT* al) {
    for (; first != last; ++first)
        destroy_one(first, al);
}

int main() {
    Pair arr[4];
    AllocT al; al.tag = 77;
    Destroy_range(arr, arr + 4, &al);

    int ok = 1;
    if (g_calls != 4) ok = 0;
    for (int i = 0; i < 4; ++i)
        if (g_ptrs[i] != &arr[i]) ok = 0;
    if (g_al != &al) ok = 0;

    // empty range: no calls
    g_calls = 0;
    Destroy_range(arr, arr, &al);
    if (g_calls != 0) ok = 0;

    if (ok) printf("PARITY_OK_006556d3\n");
    else printf("FAIL\n");
    return 0;
}