#include <cstdio>

struct CLandscapeBackgroundPatch {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CLandscapeBackgroundPatch* g_last_dtor = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self)
{
    g_last_dtor = self;
    self->destroyed = 1;
}

void free_engine(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        free_engine(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CLandscapeBackgroundPatch b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}