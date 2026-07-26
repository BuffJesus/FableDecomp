#include <cstdio>

static int g_dtor_called = 0;
static int g_free_called = 0;
static void* g_last_self = 0;

extern "C" void __cdecl eng_free(void* p) { g_free_called++; g_last_self = p; }
void __fastcall CLandscapeBackgroundPatch_dtor(void* self) { g_dtor_called++; g_last_self = self; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int edx, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        eng_free(self);
    return self;
}

int main()
{
    int dummy = 0;
    void* self = &dummy;

    g_dtor_called = 0; g_free_called = 0;
    void* r1 = CLandscapeBackgroundPatch_vector_deleting_destructor(self, 0, 0);
    if (r1 != self || g_dtor_called != 1 || g_free_called != 0) {
        std::printf("FAIL flags0 r=%p dtor=%d free=%d\n", r1, g_dtor_called, g_free_called);
        return 1;
    }

    g_dtor_called = 0; g_free_called = 0;
    void* r2 = CLandscapeBackgroundPatch_vector_deleting_destructor(self, 0, 1);
    if (r2 != self || g_dtor_called != 1 || g_free_called != 1) {
        std::printf("FAIL flags1 r=%p dtor=%d free=%d\n", r2, g_dtor_called, g_free_called);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00457116_TEST PASS\n");
    return 0;
}