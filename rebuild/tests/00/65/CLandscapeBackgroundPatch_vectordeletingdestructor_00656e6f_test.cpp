#include <cstdio>

// Behaviour model of the scalar/vector deleting destructor:
//   run inner dtor, optionally free (flag bit0), return this.

static int g_inner_dtor_calls = 0;
static int g_free_calls = 0;
static void* g_freed = 0;

struct Inner { };
static void inner_dtor(Inner*) { ++g_inner_dtor_calls; }

struct Obj {
    char pad[0x10];
    Inner inner;
};

static void my_free(void* p) { ++g_free_calls; g_freed = p; }

static void* deleting_dtor(Obj* self, unsigned int flags) {
    inner_dtor(&self->inner);
    if (flags & 1)
        my_free(self);
    return self;
}

int main() {
    Obj a;
    // flags without bit0: dtor runs, no free
    g_inner_dtor_calls = g_free_calls = 0; g_freed = 0;
    void* r = deleting_dtor(&a, 0);
    bool ok = (r == &a) && g_inner_dtor_calls == 1 && g_free_calls == 0;

    // flags with bit0: dtor runs AND free
    g_inner_dtor_calls = g_free_calls = 0; g_freed = 0;
    Obj b;
    r = deleting_dtor(&b, 1);
    ok = ok && (r == &b) && g_inner_dtor_calls == 1 && g_free_calls == 1 && g_freed == &b;

    // bit0 clear but other bits set: no free
    g_inner_dtor_calls = g_free_calls = 0;
    Obj c;
    r = deleting_dtor(&c, 2);
    ok = ok && (r == &c) && g_free_calls == 0;

    if (ok) printf("VDD_PARITY_OK\n");
    else    printf("VDD_PARITY_FAIL\n");
    return 0;
}