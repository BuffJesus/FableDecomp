#include <cstdio>
#include <cstring>
#include <new>

struct CThingSearchTools;

struct Sub {
    virtual void  s0() {}
    virtual void  s1() {}
    virtual void  s2() {}
    virtual void  s3() {}
    virtual void  s4() {}
    virtual void  s5() {}
    virtual void  s6() {}
    virtual void  s7() {}
    virtual void* slot8(void* arg) { return 0; }
};
struct Inner {
    char pad50[0x50];
    Sub  sub;
};
struct Field4 { void* pad; Inner* inner; };

extern void* const s_vtbl_12b33f0[16] = {0};

struct CAIStateGroupBase {
    void** vptr;
    Field4* field4;
    CThingSearchTools* PeekThingSearchTools();
};

static void* g_seen_arg = 0;
static void* g_ret_val  = (void*)0xDEADBEEF;

struct MySub : Sub {
    virtual void* slot8(void* arg) { g_seen_arg = arg; return g_ret_val; }
};

int main() {
    char buf[sizeof(Inner) + 64];
    memset(buf, 0, sizeof(buf));
    Inner* inner = reinterpret_cast<Inner*>(buf);
    new (&inner->sub) MySub();

    Field4 f4;
    f4.inner = inner;

    CAIStateGroupBase self;
    self.field4 = &f4;
    self.vptr = 0;

    CThingSearchTools* r = self.PeekThingSearchTools();

    bool ok = true;
    if (self.vptr != (void**)s_vtbl_12b33f0) ok = false;
    if (g_seen_arg != (void*)&self) ok = false;
    if ((void*)r != g_ret_val) ok = false;

    if (ok) printf("PEEKTST_OK\n");
    else    printf("PEEKTST_FAIL vptr=%p arg=%p ret=%p\n", (void*)self.vptr, g_seen_arg, (void*)r);
    return ok ? 0 : 1;
}