#include <cstdio>

struct CTCTavernGame {
    char pad0[0xc];
    int  field_c;
    int  field_10;
};

bool __fastcall IsBetting(CTCTavernGame *self)
{
    return self->field_10 != 0 || self->field_c >= 0;
}

int main()
{
    CTCTavernGame o;
    bool ok = true;

    o.field_c = -1; o.field_10 = 0;  ok &= (IsBetting(&o) == false);
    o.field_c = -5; o.field_10 = 0;  ok &= (IsBetting(&o) == false);
    o.field_c = 0;  o.field_10 = 0;  ok &= (IsBetting(&o) == true);
    o.field_c = 5;  o.field_10 = 0;  ok &= (IsBetting(&o) == true);
    o.field_c = -1; o.field_10 = 1;  ok &= (IsBetting(&o) == true);

    if (ok) printf("00c3c980_TEST PASS\n");
    else    printf("00c3c980_TEST FAIL\n");
    return 0;
}