#include <stdio.h>
#include <new>

struct CLandscapeBackgroundPatch { int marker; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

// definition of the extern scalar destructor the source calls
extern void __fastcall CLandscapeBackgroundPatch_scalar_destructor(CLandscapeBackgroundPatch* self);
void __fastcall CLandscapeBackgroundPatch_scalar_destructor(CLandscapeBackgroundPatch* self)
{
    (void)self;
    ++g_dtor_calls;
}

// tracked replacement operator delete
static void* g_last_delete = 0;
void operator delete(void* p) throw()
{
    g_last_delete = p;
    ++g_delete_calls;
    // no real free; test objects are stack/static
}

extern void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int edx, unsigned int flags);

int main()
{
    CLandscapeBackgroundPatch a;
    a.marker = 7;

    // flags bit0 clear: dtor runs, no delete, returns self
    g_dtor_calls = g_delete_calls = 0; g_last_delete = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0, 0);
    bool ok1 = (r1 == &a) && (g_dtor_calls == 1) && (g_delete_calls == 0);

    // flags bit0 set: dtor runs, delete called with self, returns self
    g_dtor_calls = g_delete_calls = 0; g_last_delete = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0, 1);
    bool ok2 = (r2 == &a) && (g_dtor_calls == 1) && (g_delete_calls == 1) && (g_last_delete == &a);

    // even flags value with bit0 clear (2): no delete
    g_dtor_calls = g_delete_calls = 0; g_last_delete = 0;
    void* r3 = CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0, 2);
    bool ok3 = (r3 == &a) && (g_dtor_calls == 1) && (g_delete_calls == 0);

    // odd flags value with bit0 set (3): delete
    g_dtor_calls = g_delete_calls = 0; g_last_delete = 0;
    void* r4 = CLandscapeBackgroundPatch_vector_deleting_destructor(&a, 0, 3);
    bool ok4 = (r4 == &a) && (g_dtor_calls == 1) && (g_delete_calls == 1);

    if (ok1 && ok2 && ok3 && ok4) {
        printf("VDTOR_OK\n");
        return 0;
    }
    printf("VDTOR_FAIL %d %d %d %d\n", (int)ok1, (int)ok2, (int)ok3, (int)ok4);
    return 1;
}