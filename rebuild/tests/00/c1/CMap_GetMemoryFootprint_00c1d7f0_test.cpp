#include <cstdio>

struct CMap {
    int   field0;
    int   field4;
    long  field8;
    long GetSubMemA();
    long GetSubMemB();
    long GetMemoryFootprint();
};

long CMap::GetSubMemA() { return 100 + this->field0; }
long CMap::GetSubMemB() { return 200 + this->field4; }

long CMap::GetMemoryFootprint()
{
    long total = this->GetSubMemA();
    total += this->GetSubMemB();
    return this->field8 + total;
}

int main()
{
    CMap m;
    m.field0 = 7;
    m.field4 = 13;
    m.field8 = 1000;
    long r = m.GetMemoryFootprint();
    // expect (100+7) + (200+13) + 1000 = 1320
    if (r == 1320) {
        printf("FOOTPRINT_OK\n");
        return 0;
    }
    printf("FOOTPRINT_BAD %ld\n", r);
    return 1;
}