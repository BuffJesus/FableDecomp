#include <cstdio>

struct CInputProcessInventoryBase {
    char pad[0x28];
    int m28;
    int m2c;
    char m30;
    void Init();
};

void CInputProcessInventoryBase::Init()
{
    m30 = 0;
    m28 = 0;
    m2c = 0;
}

int main()
{
    CInputProcessInventoryBase o;
    o.m28 = 0x11111111;
    o.m2c = 0x22222222;
    o.m30 = 0x33;
    o.Init();
    if (o.m28 == 0 && o.m2c == 0 && o.m30 == 0) {
        printf("INIT_ZERO_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}