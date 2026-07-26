
#include <cstdio>
#include <cstdlib>
// Behavioural model of the 70-byte retail unit at 0044ff31:
//   half A = `vector_deleting_destructor'(this, flags):
//              this->vfptr = &vtblA; dtor(this); if (flags&1) operator delete(this); return this;
//   half B = constructor-closure: p = operator new(0x2c); if(p){ ctor(p); p->vfptr=&vtblB; } return p;
struct CLandscapeBackgroundPatch { void* vfptr; };
static int g_dtor = 0, g_del = 0, g_ctor = 0;
static void* g_vtblA;
static void* g_vtblB;

void dtor(CLandscapeBackgroundPatch* p) { g_dtor++; }
void ctor(CLandscapeBackgroundPatch* p) { g_ctor++; }

void* vdd(CLandscapeBackgroundPatch* self, unsigned int flags) {
    self->vfptr = &g_vtblA;
    dtor(self);
    if (flags & 1) { g_del++; free(self); }
    return self;
}

void* closure() {
    void* raw = std::malloc(0x2c);
    CLandscapeBackgroundPatch* p = (CLandscapeBackgroundPatch*)raw;
    if (!p) return 0;
    ctor(p);
    p->vfptr = &g_vtblB;
    return p;
}

int main() {
    // exercise half A, non-deleting
    CLandscapeBackgroundPatch* obj = (CLandscapeBackgroundPatch*)std::malloc(sizeof(CLandscapeBackgroundPatch));
    void* r = vdd(obj, 0);
    if (r != obj || g_dtor != 1 || g_del != 0) { std::printf("FAIL A0\n"); return 1; }
    // exercise half A, deleting
    void* r2 = vdd(obj, 1);
    if (r2 != obj || g_dtor != 2 || g_del != 1) { std::printf("FAIL A1\n"); return 1; }
    // exercise half B
    void* c = closure();
    if (c == 0 || g_ctor != 1) { std::printf("FAIL B\n"); return 1; }
    if (((CLandscapeBackgroundPatch*)c)->vfptr != &g_vtblB) { std::printf("FAIL Bvtbl\n"); return 1; }
    std::free(c);
    std::printf("CLandscapeBackgroundPatch_0044ff31_TEST PASS\n");
    return 0;
}