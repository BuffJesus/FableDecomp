#include <stdio.h>

struct CDefClassBase {
    char pad[0x28];
    unsigned long field28;
};

struct CDrunkennessDef : public CDefClassBase {
};

static int g_baseCalled = 0;

void __fastcall BaseClassCopyImpl(CDefClassBase* self, int /*edx*/, CDefClassBase const* src)
{
    g_baseCalled = 1;
    (void)self; (void)src;
}

void __fastcall CDrunkennessDef_Copy(CDrunkennessDef* self, int /*edx*/, CDefClassBase const* src)
{
    BaseClassCopyImpl(self, 0, src);
    self->field28 = src->field28;
}

int main()
{
    CDrunkennessDef dst;
    CDefClassBase src;
    dst.field28 = 0;
    src.field28 = 0xDEADBEEF;
    CDrunkennessDef_Copy(&dst, 0, &src);
    if (g_baseCalled == 1 && dst.field28 == 0xDEADBEEF) {
        printf("004e0f1b_TEST PASS\n");
    } else {
        printf("FAIL base=%d f28=%08lx\n", g_baseCalled, dst.field28);
    }
    return 0;
}