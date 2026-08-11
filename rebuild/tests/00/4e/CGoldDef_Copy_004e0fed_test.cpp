#include <cstdio>

struct CDefClassBase { char pad[0x2c]; };

struct CGoldDef {
    char pad[0x28];
    unsigned long field28;
    void BaseClone(const CDefClassBase* other);
};

static int g_baseCalled = 0;
void CGoldDef::BaseClone(const CDefClassBase* other)
{
    g_baseCalled = 1;
    (void)other;
}

void __fastcall Copy(CGoldDef* self, int /*edx*/, const CDefClassBase* other);

int main()
{
    CGoldDef src; CGoldDef dst;
    src.field28 = 0xDEADBEEF;
    dst.field28 = 0;
    Copy(&dst, 0, (const CDefClassBase*)&src);
    if (g_baseCalled == 1 && dst.field28 == 0xDEADBEEF)
        printf("004e0fed_TEST PASS\n");
    else
        printf("FAIL base=%d f=%08lx\n", g_baseCalled, dst.field28);
    return 0;
}