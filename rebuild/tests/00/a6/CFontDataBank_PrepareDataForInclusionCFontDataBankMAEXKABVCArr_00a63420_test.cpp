// Independent behaviour model for the refcount-release logic.
#include <stdio.h>

struct RefBlock {
    int   refcount;
    void  (*dtor)(void*);
    void* payload;
};

static int g_dtor_called = 0;
static int g_freed = 0;
static void my_dtor(void*) { g_dtor_called++; }
static void my_free(void* p) { (void)p; g_freed++; }

static void release(RefBlock* p) {
    if (p) {
        if (--p->refcount == 0) {
            p->dtor(p->payload);
            my_free(p);
        }
    }
}

int main() {
    // null -> no-op
    release(0);

    // refcount 2 -> dec to 1, no dtor/free
    RefBlock b;
    b.refcount = 2; b.dtor = my_dtor; b.payload = (void*)0x1234;
    release(&b);
    if (b.refcount != 1 || g_dtor_called != 0 || g_freed != 0) { printf("BAD1\n"); return 1; }

    // dec to 0 -> dtor + free
    release(&b);
    if (b.refcount != 0 || g_dtor_called != 1 || g_freed != 1) { printf("BAD2\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}