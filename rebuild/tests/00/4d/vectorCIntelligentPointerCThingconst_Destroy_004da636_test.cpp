// Independent behaviour model for _Destroy loop.
#include <cstdio>
#include <cstring>

static int g_calls = 0;
static int g_args_sum = 0;

struct Elem {
    void* vptr;
    char pad[0x44 - 4];
};

// simulate the virtual slot0(int)
static void vslot0(Elem* self, int a) {
    (void)self;
    g_calls++;
    g_args_sum += a;
}

// re-implementation matching the retail loop shape
static void destroy(Elem* first, Elem* last) {
    for (; first != last; first = (Elem*)((char*)first + 0x44)) {
        vslot0(first, 0);
    }
}

int main() {
    const int N = 5;
    Elem arr[N];
    std::memset(arr, 0, sizeof(arr));

    // sizeof(Elem) must be 0x44 for the stride model
    if (sizeof(Elem) != 0x44) {
        std::printf("FAIL size %d\n", (int)sizeof(Elem));
        return 1;
    }

    destroy(&arr[0], &arr[N]);

    if (g_calls == N && g_args_sum == 0) {
        std::printf("DESTROY_OK calls=%d\n", g_calls);
        return 0;
    }
    std::printf("FAIL calls=%d sum=%d\n", g_calls, g_args_sum);
    return 1;
}