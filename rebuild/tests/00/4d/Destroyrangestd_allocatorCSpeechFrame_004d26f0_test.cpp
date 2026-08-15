#include <stdio.h>

// Behaviour model of _Destroy_range: destroy each element [first,last).
struct Frame { int id; int destroyed; };

static int g_calls = 0;

static void frame_dtor(Frame* f) {
    f->destroyed = 1;
    g_calls++;
}

static void destroy_range(Frame* first, Frame* last) {
    for (; first != last; ++first) {
        frame_dtor(first);
    }
}

int main() {
    Frame arr[5];
    for (int i = 0; i < 5; ++i) { arr[i].id = i; arr[i].destroyed = 0; }

    // Empty range: no calls.
    destroy_range(arr, arr);
    if (g_calls != 0) { printf("FAIL empty\n"); return 1; }

    // Full range: all destroyed.
    destroy_range(arr, arr + 5);
    if (g_calls != 5) { printf("FAIL count %d\n", g_calls); return 1; }
    for (int i = 0; i < 5; ++i) {
        if (!arr[i].destroyed) { printf("FAIL elem %d\n", i); return 1; }
    }

    printf("DESTROY_RANGE_OK\n");
    return 0;
}