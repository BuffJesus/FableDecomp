#include <cstdio>

// Independent reimplementation mirroring the authored logic.
struct Obj {
    int tag;
    int destroyed;
    int freed;
};

static Obj* g_last = 0;

static void Destroy(Obj* self) { self->destroyed = 1; }
static void FreeThis(Obj* self) { self->freed = 1; g_last = self; }

static void* vecdeldtor(Obj* self, unsigned int flags)
{
    Destroy(self);
    if (flags & 1)
        FreeThis(self);
    return self;
}

int main()
{
    int ok = 1;

    Obj a = {7, 0, 0};
    void* r = vecdeldtor(&a, 0);        // flag not set: destroy only
    if (r != &a) ok = 0;
    if (a.destroyed != 1) ok = 0;
    if (a.freed != 0) ok = 0;

    Obj b = {9, 0, 0};
    g_last = 0;
    r = vecdeldtor(&b, 1);              // flag set: destroy + free
    if (r != &b) ok = 0;
    if (b.destroyed != 1) ok = 0;
    if (b.freed != 1) ok = 0;
    if (g_last != &b) ok = 0;

    Obj c = {3, 0, 0};
    r = vecdeldtor(&c, 2);             // bit0 clear: no free
    if (c.freed != 0) ok = 0;

    if (ok) printf("VDTOR_OK_009a0ae0\n");
    else    printf("VDTOR_FAIL\n");
    return ok ? 0 : 1;
}