#include <cstdio>

struct Inner {
    long captured;
    void add(long arg);
};

struct Obj {
    unsigned int flags;   // +0x00
    char pad[4];          // +0x04
    Inner inner;          // +0x08
};

static long g_last = -1;
void Inner::add(long arg) { this->captured = arg; g_last = arg; }

void __fastcall bind2nd_greater(Obj* self, int /*edx*/, long arg)
{
    self->inner.add(arg);
    self->flags |= 0x20;
}

int main()
{
    Obj o;
    o.flags = 0x01;
    o.inner.captured = 0;
    bind2nd_greater(&o, 0, 12345L);

    bool ok = (g_last == 12345L) && (o.inner.captured == 12345L)
              && (o.flags == (0x01u | 0x20u));
    if (ok) printf("00c20440_TEST PASS\n");
    else printf("FAIL flags=%x cap=%ld last=%ld\n", o.flags, o.inner.captured, g_last);
    return 0;
}