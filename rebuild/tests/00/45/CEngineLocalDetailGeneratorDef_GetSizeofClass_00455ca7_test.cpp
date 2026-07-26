#include <cstdio>

struct CEngineLocalDetailGeneratorDef { int dummy; };

unsigned long __fastcall CEngineLocalDetailGeneratorDef_GetSizeofClass(const CEngineLocalDetailGeneratorDef* self)
{
    (void)self;
    return 0x48;
}

int main()
{
    CEngineLocalDetailGeneratorDef obj;
    unsigned long r = CEngineLocalDetailGeneratorDef_GetSizeofClass(&obj);
    if (r == 0x48UL) {
        std::printf("CEngineLocalDetailGeneratorDef_00455ca7_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}