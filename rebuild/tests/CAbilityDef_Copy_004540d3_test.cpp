#include <cstdio>

struct CAbilityDef {
    unsigned char _tail[0x2c];
    void BaseCopy(const CAbilityDef* other);
};

static int g_basecopy_called = 0;
void CAbilityDef::BaseCopy(const CAbilityDef* other)
{
    (void)other;
    g_basecopy_called = 1;
}

void __fastcall CAbilityDef_Copy(CAbilityDef* self, void* dummy, const CAbilityDef* other)
{
    (void)dummy;
    self->BaseCopy(other);
    *(unsigned long*)(self->_tail + 0x28) = *(const unsigned long*)(other->_tail + 0x28);
}

int main()
{
    CAbilityDef dst, src;
    for (int i = 0; i < 0x2c; ++i) { dst._tail[i] = 0; src._tail[i] = 0; }
    *(unsigned long*)(src._tail + 0x28) = 0xDEADBEEFu;
    CAbilityDef_Copy(&dst, 0, &src);
    if (g_basecopy_called != 1) { std::printf("FAIL base not called\n"); return 1; }
    if (*(unsigned long*)(dst._tail + 0x28) != 0xDEADBEEFu) { std::printf("FAIL field\n"); return 1; }
    std::printf("CAbilityDef_004540d3_TEST PASS\n");
    return 0;
}