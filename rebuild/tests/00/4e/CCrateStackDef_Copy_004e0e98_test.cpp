#include <stdio.h>

struct Base { int called; };
struct Def {
    char pad[0x28];
    unsigned int m28;
    unsigned int m2c;
};

static int g_base_called = 0;
static void BaseCopy(Def* self, const Def* other) { (void)self; (void)other; g_base_called = 1; }

static void Copy(Def* self, const Def* other)
{
    BaseCopy(self, other);
    self->m28 = other->m28;
    self->m2c = other->m2c;
}

int main()
{
    Def src; Def dst;
    for (int i = 0; i < (int)sizeof(Def); ++i) ((char*)&dst)[i] = 0;
    src.m28 = 0xAABBCCDD;
    src.m2c = 0x11223344;
    Copy(&dst, &src);
    if (g_base_called == 1 && dst.m28 == 0xAABBCCDD && dst.m2c == 0x11223344)
        printf("PARITY_OK\n");
    else
        printf("FAIL\n");
    return 0;
}