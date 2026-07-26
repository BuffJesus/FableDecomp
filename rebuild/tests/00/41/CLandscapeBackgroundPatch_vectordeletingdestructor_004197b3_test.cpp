#include <cstdio>

// Standalone behavioural model of CLandscapeBackgroundPatch's
// `vector_deleting_destructor'. We reproduce the thunk's observable logic:
// run the destructor, then free the object iff (flags & 1); always return self.

static int g_dtor_calls = 0;
static int g_free_calls = 0;

struct CLandscapeBackgroundPatch {
    void* vftable;
    int   tag;
};

static void CLBP_cleanup(void* p) {
    (void)p;
    ++g_dtor_calls;
}

// Model of the vector deleting destructor thunk.
static void* vecDelDtor(CLandscapeBackgroundPatch* self, unsigned int flags) {
    CLBP_cleanup(self);            // scalar destructor
    if (flags & 1) {
        ++g_free_calls;            // operator delete(self)
    }
    return self;
}

int main() {
    CLandscapeBackgroundPatch a; a.vftable = 0; a.tag = 7;

    // flags & 1 == 0 -> destruct only, no free, returns self
    g_dtor_calls = g_free_calls = 0;
    void* r0 = vecDelDtor(&a, 0);
    if (r0 != &a || g_dtor_calls != 1 || g_free_calls != 0) {
        std::printf("FAIL no-free path\n"); return 1;
    }

    // flags & 1 == 1 -> destruct + free, returns self
    g_dtor_calls = g_free_calls = 0;
    void* r1 = vecDelDtor(&a, 1);
    if (r1 != &a || g_dtor_calls != 1 || g_free_calls != 1) {
        std::printf("FAIL free path\n"); return 1;
    }

    // higher bits set but bit0 clear -> no free
    g_dtor_calls = g_free_calls = 0;
    void* r2 = vecDelDtor(&a, 2);
    if (r2 != &a || g_free_calls != 0) {
        std::printf("FAIL bit-mask path\n"); return 1;
    }

    std::printf("CLandscapeBackgroundPatch_004197b3_TEST PASS\n");
    return 0;
}