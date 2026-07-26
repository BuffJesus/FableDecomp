#include <cstdio>

struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    char inner[4];
    unsigned char _pad_0x10[0xa8];
    void* tex;
};

static int g_dtor_called = 0;
static int g_delete_called = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_dtor_called++; }
void __cdecl eng_operator_delete(void* p) { (void)p; g_delete_called++; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags)
{
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        eng_operator_delete(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    g_dtor_called = 0; g_delete_called = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL nodelete\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL delete\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00456948_TEST PASS\n");
    return 0;
}