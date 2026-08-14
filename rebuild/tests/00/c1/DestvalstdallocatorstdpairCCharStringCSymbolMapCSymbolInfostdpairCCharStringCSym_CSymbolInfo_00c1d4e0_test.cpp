#include <cstdio>

// Behaviour model: _Dest_val destroys 3 consecutive elements at stride 0x0c, in order 0,1,2.
struct Pair { char data[0x0c]; };

static int g_destroyed[3];
static int g_count = 0;
static Pair* g_base = 0;

static void pair_dtor(Pair* self) {
    int idx = (int)(self - g_base);
    if (idx >= 0 && idx < 3) g_destroyed[g_count] = idx;
    g_count++;
}

static void Dest_val(Pair* p) {
    pair_dtor(&p[0]);
    pair_dtor(&p[1]);
    pair_dtor(&p[2]);
}

int main() {
    Pair arr[3];
    g_base = arr;
    g_count = 0;
    Dest_val(arr);
    // Expect exactly 3 destroys, in order 0,1,2.
    if (g_count == 3 &&
        g_destroyed[0] == 0 &&
        g_destroyed[1] == 1 &&
        g_destroyed[2] == 2 &&
        sizeof(Pair) == 0x0c) {
        printf("DESTVAL_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}