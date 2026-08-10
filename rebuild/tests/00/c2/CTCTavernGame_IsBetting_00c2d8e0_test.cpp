#include <cstdio>

struct CTCTavernGame {
    unsigned int pad0;
    unsigned int a;
    unsigned int b;
};

bool __fastcall IsBetting(CTCTavernGame *self);

int main()
{
    CTCTavernGame o;
    o.pad0 = 0xDEADBEEF;
    o.a = 0; o.b = 0;
    bool r1 = IsBetting(&o);
    o.a = 5; o.b = 0;
    bool r2 = IsBetting(&o);
    o.a = 0; o.b = 7;
    bool r3 = IsBetting(&o);
    o.a = 3; o.b = 9;
    bool r4 = IsBetting(&o);
    if (!r1 && r2 && r3 && r4) {
        printf("00c2d8e0_TEST PASS\n");
    } else {
        printf("FAIL r1=%d r2=%d r3=%d r4=%d\n", r1, r2, r3, r4);
    }
    return 0;
}