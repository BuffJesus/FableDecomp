#include <cstdio>
static int g_called = 0;
struct CBase { void Copy(const CBase* other); unsigned char _b[0x30]; };
struct CGuildMasterDef : CBase { void Copy(const CGuildMasterDef* other); };
void CBase::Copy(const CBase* other) { g_called = 1; }
void CGuildMasterDef::Copy(const CGuildMasterDef* other) {
    this->CBase::Copy((const CBase*)other);
    *(unsigned long*)((char*)this + 0x28) = *(const unsigned long*)((const char*)other + 0x28);
    *(unsigned long*)((char*)this + 0x2c) = *(const unsigned long*)((const char*)other + 0x2c);
}
int main() {
    CGuildMasterDef dst, src;
    for (int i = 0; i < 0x30; ++i) { dst._b[i] = 0; src._b[i] = (unsigned char)(i + 1); }
    *(unsigned long*)((char*)&src + 0x28) = 0xAABBCCDD;
    *(unsigned long*)((char*)&src + 0x2c) = 0x11223344;
    dst.Copy(&src);
    unsigned long a = *(unsigned long*)((char*)&dst + 0x28);
    unsigned long b = *(unsigned long*)((char*)&dst + 0x2c);
    if (g_called == 1 && a == 0xAABBCCDD && b == 0x11223344) {
        std::printf("CGuildMasterDef_00454248_TEST PASS\n");
        return 0;
    }
    std::printf("CGuildMasterDef_00454248_TEST FAIL a=%08lx b=%08lx c=%d\n", a, b, g_called);
    return 1;
}