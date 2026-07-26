#include <cstdio>
#include <cstdlib>

struct CLandscapeBackgroundPatch { void* vt; int destroyed; };
static const void* VT1 = (const void*)0x1230ba0;
static int g_dtor_calls = 0;
static void dtor_body(CLandscapeBackgroundPatch* s){ g_dtor_calls++; s->destroyed = 1; }

// plain C++ equivalent of the scalar deleting destructor path
static void* dd(CLandscapeBackgroundPatch* self, unsigned char flags){
    *(const void**)self = VT1;
    dtor_body(self);
    if (flags & 1) { std::free(self); return self; }
    return self;
}

int main(){
    // flag&1 clear: no free, returns self, vtable set, dtor ran
    CLandscapeBackgroundPatch a; a.vt=0; a.destroyed=0;
    void* r = dd(&a, 0);
    if (r != &a) { std::printf("FAIL ret\n"); return 1; }
    if (a.vt != VT1) { std::printf("FAIL vt\n"); return 1; }
    if (a.destroyed != 1) { std::printf("FAIL dtor\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL calls\n"); return 1; }
    // flag&1 set: heap object, should free (exercise path)
    CLandscapeBackgroundPatch* h = (CLandscapeBackgroundPatch*)std::malloc(sizeof(*h));
    h->vt=0; h->destroyed=0;
    void* r2 = dd(h, 1);
    if (r2 != h) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor_calls != 2) { std::printf("FAIL calls2\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0044fea5_TEST PASS\n");
    return 0;
}