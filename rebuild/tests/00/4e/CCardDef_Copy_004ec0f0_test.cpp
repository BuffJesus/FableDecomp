#include <cstdio>

struct CDefClassBase;

struct CCardDef {
    char pad[0x3c];
    unsigned int f3c;
    unsigned int f40;
};

static int g_basecalled = 0;
extern "C" void __fastcall BaseCopy(CCardDef* self, int edx_, CDefClassBase const* src)
{
    (void)self; (void)edx_; (void)src;
    g_basecalled = 1;
}

void __fastcall CCardDef_Copy(CCardDef* self, int edx_, CDefClassBase const* src)
{
    BaseCopy(self, edx_, src);
    self->f3c = ((CCardDef const*)src)->f3c;
    self->f40 = ((CCardDef const*)src)->f40;
}

int main()
{
    CCardDef dst; CCardDef src;
    for (int i = 0; i < (int)sizeof(dst); ++i) ((char*)&dst)[i] = 0;
    src.f3c = 0x11223344;
    src.f40 = 0x55667788;
    CCardDef_Copy(&dst, 0, (CDefClassBase const*)&src);
    if (g_basecalled == 1 && dst.f3c == 0x11223344u && dst.f40 == 0x55667788u)
        printf("PARITY_OK\n");
    else
        printf("FAIL\n");
    return 0;
}