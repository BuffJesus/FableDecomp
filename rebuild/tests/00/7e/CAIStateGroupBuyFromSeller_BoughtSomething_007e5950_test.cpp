#include <cstdio>

int g_buyFromSellerInt = 3;

struct CAIStateGroup_BuyFromSeller
{
    char pad[0xC];
    int  m_result; // +0xC
    void BoughtSomething(float amount);
};

void CAIStateGroup_BuyFromSeller::BoughtSomething(float amount)
{
    this->m_result = (int)((float)g_buyFromSellerInt * amount);
}

int main()
{
    CAIStateGroup_BuyFromSeller o;
    o.m_result = -999;

    g_buyFromSellerInt = 3;
    o.BoughtSomething(4.0f);      // 3*4 = 12
    if (o.m_result != 12) { printf("FAIL a %d\n", o.m_result); return 1; }

    g_buyFromSellerInt = 10;
    o.BoughtSomething(2.5f);      // 10*2.5 = 25
    if (o.m_result != 25) { printf("FAIL b %d\n", o.m_result); return 1; }

    g_buyFromSellerInt = 7;
    o.BoughtSomething(0.0f);      // 0
    if (o.m_result != 0) { printf("FAIL c %d\n", o.m_result); return 1; }

    printf("BUYFROMSELLER_OK\n");
    return 0;
}