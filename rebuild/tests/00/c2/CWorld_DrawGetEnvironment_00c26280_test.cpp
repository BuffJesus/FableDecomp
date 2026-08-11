#include <stdio.h>

struct CWorld {
    char pad[0x10];
    unsigned int m_val;
    int DrawGetEnvironment(unsigned int threshold);
};

int CWorld::DrawGetEnvironment(unsigned int threshold)
{
    return this->m_val >= threshold ? 1 : 0;
}

int main()
{
    CWorld w;
    w.m_val = 100;
    int ok = 1;
    if (w.DrawGetEnvironment(50) != 1) ok = 0;   // 100>=50 -> 1
    if (w.DrawGetEnvironment(100) != 1) ok = 0;  // equal -> 1
    if (w.DrawGetEnvironment(150) != 0) ok = 0;  // 100<150 -> 0
    w.m_val = 0xFFFFFFFFu;
    if (w.DrawGetEnvironment(0x80000000u) != 1) ok = 0; // unsigned
    w.m_val = 0x00000001u;
    if (w.DrawGetEnvironment(0x80000000u) != 0) ok = 0; // unsigned
    printf(ok ? "TEST_PASS_C26280\n" : "TEST_FAIL\n");
    return ok ? 0 : 1;
}