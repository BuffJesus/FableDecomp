#include <cstdio>

struct CPhysicsDef { int _dummy; };

unsigned long __fastcall CPhysicsDef_GetSizeofClass(const CPhysicsDef* self)
{
    (void)self;
    return 0x44;
}

int main()
{
    CPhysicsDef d;
    unsigned long v = CPhysicsDef_GetSizeofClass(&d);
    if (v == 0x44) {
        std::printf("CPhysicsDef_0044c158_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", v);
    return 1;
}