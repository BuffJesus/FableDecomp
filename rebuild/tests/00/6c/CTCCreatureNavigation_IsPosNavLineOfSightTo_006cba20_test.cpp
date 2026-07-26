#include <cstdio>
struct CPos { int v; };
struct CNavObj;
typedef bool (*VFn)(void*, float);
struct CVtbl { VFn m[16]; };
struct CNavObj { CVtbl* vptr; int base; };
struct CNavHelper { int add; };
static int g_helper_val;
static float g_arg_seen;
static bool Vm(void* self, float arg) { CNavObj* o = (CNavObj*)self; union { int i; float f; } u; u.f = arg; g_arg_seen = arg; return (o->base + u.i) != 0; }
struct CTCCreatureNavigation { void* pad0; CNavHelper* helper; void* pad8; CNavObj* obj; };
static int HelperCompute(CNavHelper* h, const CPos* p) { return h->add + p->v; }
static bool run(CTCCreatureNavigation* self, const CPos* pos) {
    CNavObj* obj = self->obj;
    CVtbl* vt = obj->vptr;
    union { int i; float f; } u;
    u.i = HelperCompute(self->helper, pos);
    g_helper_val = u.i;
    return ((VFn)vt->m[10])(self->obj, u.f);
}
int main() {
    CVtbl vt; for (int i=0;i<16;i++) vt.m[i]=(VFn)Vm;
    CNavObj obj; obj.vptr=&vt; obj.base=5;
    CNavHelper h; h.add=3;
    CTCCreatureNavigation self; self.helper=&h; self.obj=&obj;
    CPos pos; pos.v=2;
    bool b = run(&self, &pos);
    /* helper: 3+2=5, vfunc: base 5 + 5 = 10 != 0 -> true */
    if (b == true && g_helper_val == 5) { std::printf("CTCCreatureNavigation_006cba20_TEST PASS\n"); return 0; }
    std::printf("CTCCreatureNavigation_006cba20_TEST FAIL b=%d hv=%d\n", (int)b, g_helper_val); return 1;
}