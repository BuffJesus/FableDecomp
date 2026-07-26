#include <cstdio>

struct CShaderPreParser {
    char pad[0x7d0];
    char* begin;
    char* end;
};

unsigned long __fastcall CShaderPreParser_GetConstantCount(const CShaderPreParser* self)
{
    return (unsigned long)((self->end - self->begin) >> 3);
}

int main()
{
    CShaderPreParser p;
    char buf[80];
    p.begin = buf;
    p.end = buf + 8 * 5;
    if (CShaderPreParser_GetConstantCount(&p) != 5) {
        std::printf("FAIL count=%lu\n", CShaderPreParser_GetConstantCount(&p));
        return 1;
    }
    p.end = buf;
    if (CShaderPreParser_GetConstantCount(&p) != 0) {
        std::printf("FAIL empty\n");
        return 1;
    }
    std::printf("CShaderPreParser_00445960_TEST PASS\n");
    return 0;
}