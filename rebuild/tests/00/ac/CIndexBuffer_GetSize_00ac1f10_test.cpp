#include <cstdio>

struct Desc { long a, b, size, d, e; };

struct IBufImpl;
struct IBufImplVtbl {
    void *s0,*s1,*s2,*s3,*s4,*s5,*s6,*s7,*s8,*s9,*s10,*s11,*s12;
    void (__stdcall *GetDesc)(IBufImpl* self, Desc* out);
};
struct IBufImpl { IBufImplVtbl* vtbl; };
struct CIndexBuffer { void* f0; IBufImpl* f4; };

long __fastcall GetSize(CIndexBuffer* self);

static IBufImpl* g_expect;
static void __stdcall FakeGetDesc(IBufImpl* self, Desc* out)
{
    if (self != g_expect) { printf("BAD self\n"); return; }
    out->a = 111; out->b = 222; out->size = 333; out->d = 444; out->e = 4096;
}

int main()
{
    IBufImplVtbl vt;
    for (int i=0;i<13;i++) ((void**)&vt)[i]=0;
    vt.GetDesc = FakeGetDesc;
    IBufImpl impl; impl.vtbl = &vt;
    CIndexBuffer ib; ib.f0=0; ib.f4=&impl;
    g_expect=&impl;

    long r = GetSize(&ib);
    if (r == 4096) printf("00ac1f10_TEST PASS\n");
    else printf("FAIL r=%ld\n", r);
    return 0;
}