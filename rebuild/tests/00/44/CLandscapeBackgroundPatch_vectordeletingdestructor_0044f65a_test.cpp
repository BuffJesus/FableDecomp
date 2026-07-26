#include <cstdio>

// Behavioural model of CLandscapeBackgroundPatch::`vector deleting destructor'.
// (The retail body is a naked compiler thunk; here we model its observable effects.)

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_last_deleted = 0;

struct CLandscapeBackgroundPatch {
    void* vft; // +0x0
};

static void* VFT_A = (void*)0x1230ba0;

static void dtor_inner(void* p) { (void)p; ++g_dtor_calls; }
static void op_delete(void* p) { ++g_delete_calls; g_last_deleted = p; }

void* CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, unsigned int flags)
{
    ((CLandscapeBackgroundPatch*)self)->vft = VFT_A;
    dtor_inner(self);
    if (flags & 1) {
        op_delete(self);
    }
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj; obj.vft = 0;

    // flags & 1 clear: destructor runs, NO delete, returns self.
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void* r0 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0);
    if (r0 != &obj)          { std::printf("FAIL ret0\n"); return 1; }
    if (g_dtor_calls != 1)   { std::printf("FAIL dtor0\n"); return 1; }
    if (g_delete_calls != 0) { std::printf("FAIL del0\n"); return 1; }
    if (obj.vft != VFT_A)    { std::printf("FAIL vft0\n"); return 1; }

    // flags & 1 set: destructor runs, delete called on self, returns self.
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 1);
    if (r1 != &obj)             { std::printf("FAIL ret1\n"); return 1; }
    if (g_dtor_calls != 1)      { std::printf("FAIL dtor1\n"); return 1; }
    if (g_delete_calls != 1)    { std::printf("FAIL del1\n"); return 1; }
    if (g_last_deleted != &obj) { std::printf("FAIL delptr\n"); return 1; }

    // high bit set but bit0 clear -> still no delete
    g_delete_calls = 0;
    CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 2);
    if (g_delete_calls != 0) { std::printf("FAIL del2\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_0044f65a_TEST PASS\n");
    return 0;
}