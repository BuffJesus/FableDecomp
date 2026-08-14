#include <cstdio>

struct CObjC { int called; int a; unsigned int b; };
struct CObjB { char pad[0x18]; CObjC* field18; };
struct Self { int didA; };

static int g_A_called = 0;
static CObjC g_c;
static CObjB g_b;

void SubA(Self* s) { s->didA = 1; g_A_called = 1; }
CObjB* SubB(Self* s) { (void)s; return &g_b; }
void SubM(CObjC* self, int a, unsigned int b) { self->called = 1; self->a = a; self->b = b; }

void Cleanup(Self* self)
{
    SubA(self);
    CObjB* b = SubB(self);
    SubM(b->field18, 3, 0x40000000u);
}

int main()
{
    g_b.field18 = &g_c;
    Self s; s.didA = 0;
    Cleanup(&s);
    if (s.didA == 1 && g_A_called == 1 && g_c.called == 1 &&
        g_c.a == 3 && g_c.b == 0x40000000u)
        printf("CLEANUP_OK\n");
    else
        printf("CLEANUP_FAIL\n");
    return 0;
}