#include <cstdio>

struct Inner {
    char pad[0x91];
    unsigned char flag;
};
struct Obj {
    void* vtbl;
    Inner* inner;
};

static bool IsDead(Obj* self)
{
    Inner* p = self->inner;
    bool alive = (p != 0) && ((p->flag & 1) == 0);
    return !alive;
}

int main()
{
    Inner a; a.flag = 0;
    Inner b; b.flag = 1;
    Obj o;

    o.inner = 0;
    if (IsDead(&o) != true) { printf("F1\n"); return 1; }

    o.inner = &a;
    if (IsDead(&o) != false) { printf("F2\n"); return 1; }

    o.inner = &b;
    if (IsDead(&o) != true) { printf("F3\n"); return 1; }

    a.flag = 2;
    if (IsDead(&o) != true) { printf("F4\n"); return 1; }
    o.inner = &a;
    if (IsDead(&o) != false) { printf("F5\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}