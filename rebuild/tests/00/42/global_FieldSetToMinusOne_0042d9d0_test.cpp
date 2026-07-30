#include <cstdio>

struct C {
    int field0;
    int guard;
    C* SetToMinusOne();
};

C* C::SetToMinusOne()
{
    this->field0 = -1;
    return this;
}

int main()
{
    C obj;
    obj.field0 = 0x12345678;
    obj.guard  = 0x0BADF00D;
    C* r = obj.SetToMinusOne();
    if (r != &obj) { printf("FAIL ret\n"); return 1; }
    if (obj.field0 != -1) { printf("FAIL field0=%d\n", obj.field0); return 1; }
    if (obj.guard != 0x0BADF00D) { printf("FAIL guard\n"); return 1; }
    printf("OK_0x0042d9d0\n");
    return 0;
}