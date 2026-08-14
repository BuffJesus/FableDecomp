// Independent behaviour model of the counted-pointer release performed by
// CStreamingFontDataBank::PrepareDataForInclusion.
#include <cstdio>

struct RefBlock {
    long count;
    void (*release)(void* payload);
    void* payload;
};

static int g_released = 0;
static int g_freed = 0;

static void my_release(void* payload) {
    g_released++;
    (void)payload;
}
static void my_free(RefBlock* b) {
    g_freed++;
    (void)b;
}

// Re-implementation of the destruction logic.
static void destroy(RefBlock* ctrl) {
    RefBlock* b = ctrl;
    if (b) {
        if (--b->count == 0) {
            b->release(b->payload);
            my_free(b);
        }
    }
}

int main() {
    int payload = 42;

    // Case 1: null control block -> nothing happens.
    destroy(0);

    // Case 2: count > 1 -> just decrements, no release/free.
    RefBlock b2; b2.count = 3; b2.release = my_release; b2.payload = &payload;
    destroy(&b2);
    if (b2.count != 2 || g_released != 0 || g_freed != 0) { printf("BAD1\n"); return 1; }

    // Case 3: count == 1 -> hits zero, releases and frees.
    RefBlock b3; b3.count = 1; b3.release = my_release; b3.payload = &payload;
    destroy(&b3);
    if (b3.count != 0 || g_released != 1 || g_freed != 1) { printf("BAD2\n"); return 1; }

    printf("PREPINCL_OK\n");
    return 0;
}