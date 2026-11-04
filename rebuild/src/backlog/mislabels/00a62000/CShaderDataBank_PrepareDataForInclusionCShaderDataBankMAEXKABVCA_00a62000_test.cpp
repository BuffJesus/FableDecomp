#include <stdio.h>

struct RefBlock {
    long refs;
    void (*dtor)(void*);
    void* target;
};

static int g_dtorCalled = 0;
static int g_freed = 0;
static void* g_dtorArg = 0;

static void my_dtor(void* self) { g_dtorCalled++; g_dtorArg = self; }
static void my_free(void* p) { (void)p; g_freed++; }

static void release(RefBlock* ptr) {
    if (ptr) {
        if (--ptr->refs == 0) {
            ptr->dtor(ptr->target);
            my_free(ptr);
        }
    }
}

int main() {
    release(0);
    if (g_dtorCalled != 0 || g_freed != 0) { printf("FAIL null\n"); return 1; }

    void* tgt = (void*)0x1234;
    RefBlock b2 = { 2, my_dtor, tgt };
    release(&b2);
    if (b2.refs != 1 || g_dtorCalled != 0 || g_freed != 0) { printf("FAIL refs2\n"); return 1; }

    RefBlock b1 = { 1, my_dtor, tgt };
    release(&b1);
    if (b1.refs != 0 || g_dtorCalled != 1 || g_freed != 1 || g_dtorArg != tgt) {
        printf("FAIL refs1\n"); return 1;
    }

    printf("PDFI_PARITY_OK\n");
    return 0;
}