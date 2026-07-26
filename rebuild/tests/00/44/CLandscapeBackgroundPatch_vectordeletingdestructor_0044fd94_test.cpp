#include <cstdio>

struct CLandscapeBackgroundPatch {
    void* vtbl;
    int f4;
    void* f8;
    char pad0[0x28-0xc];
    void* vtbl2;
    char pad1[0x44-0x2c];
};

static int g_dtor_called = 0;
static int g_delete_called = 0;

static void __fastcall CLandscapeBackgroundPatch_dtor_inner(CLandscapeBackgroundPatch* self){ g_dtor_called++; (void)self; }
static void op_delete(void* p){ g_delete_called++; (void)p; }

static int vtblA = 0x1232484;
static int vtblB = 0x1230ba0;

static void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, unsigned int flags)
{
    self->vtbl2 = (void*)&vtblA;
    self->vtbl  = (void*)&vtblB;
    CLandscapeBackgroundPatch_dtor_inner(self);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main(){
    CLandscapeBackgroundPatch o;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&o, 0);
    if (r != &o || g_dtor_called != 1 || g_delete_called != 0){ std::printf("FAIL a\n"); return 1; }
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&o, 1);
    if (r != &o || g_dtor_called != 2 || g_delete_called != 1){ std::printf("FAIL b\n"); return 1; }
    if (o.vtbl != (void*)&vtblB || o.vtbl2 != (void*)&vtblA){ std::printf("FAIL c\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0044fd94_TEST PASS\n");
    return 0;
}