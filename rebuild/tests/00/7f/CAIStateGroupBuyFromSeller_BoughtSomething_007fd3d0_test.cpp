#include <stdio.h>

int g_scale = 0;

struct BuyFromSeller
{
    char pad[0x24];
    int  field_24;
    void BoughtSomething(float amount)
    {
        this->field_24 = (int)((float)g_scale * amount);
    }
};

int main()
{
    BuyFromSeller obj;
    obj.field_24 = -999;

    // scale 0 -> always 0
    g_scale = 0;
    obj.BoughtSomething(5.5f);
    if (obj.field_24 != 0) { printf("FAIL a %d\n", obj.field_24); return 1; }

    // scale 10, amount 2.5 -> 25
    g_scale = 10;
    obj.BoughtSomething(2.5f);
    if (obj.field_24 != 25) { printf("FAIL b %d\n", obj.field_24); return 1; }

    // truncation toward zero: 10 * 0.19 = 1.9 -> 1
    g_scale = 10;
    obj.BoughtSomething(0.19f);
    if (obj.field_24 != 1) { printf("FAIL c %d\n", obj.field_24); return 1; }

    // negative amount: 4 * -3.0 = -12
    g_scale = 4;
    obj.BoughtSomething(-3.0f);
    if (obj.field_24 != -12) { printf("FAIL d %d\n", obj.field_24); return 1; }

    printf("BOUGHTSOMETHING_OK\n");
    return 0;
}