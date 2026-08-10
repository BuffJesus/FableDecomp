#include <cstdio>

struct Inner {
    long val;
    void Store(long a); // __fastcall -> this in ECX, a on stack
};

struct Obj {
    unsigned long flags;   // +0x00
    long          pad;     // +0x04
    Inner         second;  // +0x08
};

static Inner* g_this = 0;
static long   g_a = 0;
static int    g_calls = 0;

void Inner::Store(long a)
{
    g_this = this; g_a = a; ++g_calls;
    this->val = a;
}

void __fastcall Recon(Obj* self, int /*edx*/, long a)
{
    self->second.Store(a);
    self->flags |= 0x40;
}

int main()
{
    Obj o;
    o.flags = 0x01;
    o.pad = 0x11;
    o.second.val = 0x22;

    Recon(&o, 0, 0x1234);

    bool ok = true;
    ok &= (g_calls == 1);
    ok &= (g_this == &o.second);
    ok &= (g_a == 0x1234);
    ok &= (o.second.val == 0x1234);
    ok &= (o.flags == (0x01 | 0x40));
    ok &= (o.pad == 0x11);

    if (ok) printf("00c20560_TEST PASS\n");
    else    printf("FAIL calls=%d a=%ld val=%ld flags=%lu\n",
                   g_calls, g_a, o.second.val, o.flags);
    return 0;
}