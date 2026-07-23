#include <cstdio>
static int g_arg = 0; static int g_ok = 0;
struct CBase {
    int vt;
    int base1, base2;
    CBase(int arg) { g_arg = arg; g_ok = 1; base1 = 0; base2 = 0; }
};
struct CTCPhysicsBase : public CBase {
    int f0c, f10, f14, f18, f1c, f20, f24, f28, f2c, f30;
    int f34;
    int f38;
    unsigned char f3c;
    CTCPhysicsBase(int arg);
};
CTCPhysicsBase::CTCPhysicsBase(int arg) : CBase(arg)
{
    volatile int* p = (volatile int*)this;
    p[0] = 0x0125d2c4;
    p[3] = 0; p[4] = 0; p[5] = 0; p[6] = 0; p[7] = 0;
    p[8] = 0; p[9] = 0; p[10] = 0; p[11] = 0; p[12] = 0;
    p[13] = 0xbf800000;
    p[14] = 0;
    f3c &= 0xfc;
}
int main()
{
    CTCPhysicsBase o(7);
    if (g_arg != 7) { std::printf("FAIL arg\n"); return 1; }
    if (o.vt != 0x0125d2c4) { std::printf("FAIL vt\n"); return 1; }
    if (o.f0c || o.f30 || o.f38) { std::printf("FAIL zero\n"); return 1; }
    if ((unsigned)o.f34 != 0xbf800000u) { std::printf("FAIL f34\n"); return 1; }
    std::printf("CTCPhysicsBase_006b0730_TEST PASS\n");
    return 0;
}