#include <cstdio>

struct CMap {
    char pad0[8];
    long m8;
    long BaseFootprintA();
    long BaseFootprintB();
    long GetMemoryFootprint();
};

static long g_a, g_b;
long CMap::BaseFootprintA() { return g_a; }
long CMap::BaseFootprintB() { return g_b; }

long CMap::GetMemoryFootprint()
{
    long a = this->BaseFootprintA();
    a += this->BaseFootprintB();
    return a + this->m8;
}

int main()
{
    CMap m;
    m.m8 = 100;
    g_a = 7; g_b = 20;
    long r = m.GetMemoryFootprint();
    if (r == 127) {
        printf("FOOTPRINT_OK\n");
        return 0;
    }
    printf("FAIL %ld\n", r);
    return 1;
}