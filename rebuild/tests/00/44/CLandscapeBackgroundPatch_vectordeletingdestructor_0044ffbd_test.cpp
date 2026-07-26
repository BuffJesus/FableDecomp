#include <cstdio>
#include <cstdlib>

// Behavioral model of the two contiguous thunks at 0044ffbd.

struct Obj { void* vtbl; char pad[0x50]; };

static void* VT_DTOR = (void*)0x1230ba0;
static void* VT_CTOR = (void*)0x12329b4;

static int g_dtor=0, g_free=0, g_ctor=0;
static void dtor(Obj* o){ (void)o; g_dtor=1; }
static void freefn(Obj* o){ (void)o; g_free=1; }
static void ctor(Obj* o){ (void)o; g_ctor=1; }

// path A: vector deleting destructor
void* vdd(Obj* self, unsigned flags){
    self->vtbl = VT_DTOR;
    dtor(self);
    if (flags & 1) freefn(self);
    return self;
}
// path B: allocating default constructor closure
void* ctor_closure(){
    Obj* p = (Obj*)std::malloc(0x54);   // operator new(0x54)
    if (!p) return 0;
    ctor(p);
    p->vtbl = VT_CTOR;
    return p;
}

int main(){
    Obj o; o.vtbl=0;
    void* r = vdd(&o, 1);
    bool ok = (r==&o && o.vtbl==VT_DTOR && g_dtor && g_free);

    g_dtor=g_free=0;
    void* r2 = vdd(&o, 0);
    ok = ok && (r2==&o && g_dtor && !g_free);

    void* c = ctor_closure();
    ok = ok && (c!=0 && ((Obj*)c)->vtbl==VT_CTOR && g_ctor);
    std::free(c);

    if (ok){ std::printf("CLandscapeBackgroundPatch_0044ffbd_TEST PASS\n"); return 0; }
    std::printf("FAIL\n");
    return 1;
}