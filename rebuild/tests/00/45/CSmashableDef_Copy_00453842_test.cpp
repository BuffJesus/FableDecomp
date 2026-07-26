#include <cstdio>

struct CBaseDef {
    char first;
    void Copy(const CBaseDef* src);
};
static int g_base_called = 0;
void CBaseDef::Copy(const CBaseDef* src) { (void)src; g_base_called = 1; }

struct CSmashableDef : public CBaseDef {
    char pad[0x25];
    unsigned char b25;
    char pad2[0x28 - 0x26];
    int d28;
    int d2c;
};

void __fastcall CSmashableDef_Copy(CSmashableDef* self, void* edx, const CSmashableDef* src)
{
    self->Copy(src);
    self->b25 = src->b25;
    self->d28 = src->d28;
    self->d2c = src->d2c;
}

int main()
{
    CSmashableDef dst; CSmashableDef s;
    s.b25 = 0xAB; s.d28 = 0x11223344; s.d2c = 0x55667788;
    dst.b25 = 0; dst.d28 = 0; dst.d2c = 0;
    CSmashableDef_Copy(&dst, 0, &s);
    if (g_base_called && dst.b25 == 0xAB && dst.d28 == 0x11223344 && dst.d2c == 0x55667788) {
        std::printf("CSmashableDef_00453842_TEST PASS\n");
        return 0;
    }
    std::printf("CSmashableDef_00453842_TEST FAIL\n");
    return 1;
}