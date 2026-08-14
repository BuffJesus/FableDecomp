#include <stdio.h>

struct Obj {
    unsigned char flag;
    int helperRet;
    int called638;
};

static int helper_5bce2f(Obj* self) { return self->helperRet; }
static void helper_5bc638(Obj* self) { self->called638 = 1; }

static void ProcessInventoryInputLeft(Obj* self)
{
    if (self->flag != 0)
        return;
    if (helper_5bce2f(self) <= 0)
        return;
    helper_5bc638(self);
}

int main()
{
    // flag set -> early return, no call
    { Obj o; o.flag = 1; o.helperRet = 5; o.called638 = 0;
      ProcessInventoryInputLeft(&o);
      if (o.called638 != 0) { printf("FAIL flag\n"); return 1; } }

    // flag clear, helper <=0 -> no call
    { Obj o; o.flag = 0; o.helperRet = 0; o.called638 = 0;
      ProcessInventoryInputLeft(&o);
      if (o.called638 != 0) { printf("FAIL le0\n"); return 1; } }
    { Obj o; o.flag = 0; o.helperRet = -3; o.called638 = 0;
      ProcessInventoryInputLeft(&o);
      if (o.called638 != 0) { printf("FAIL neg\n"); return 1; } }

    // flag clear, helper >0 -> call
    { Obj o; o.flag = 0; o.helperRet = 7; o.called638 = 0;
      ProcessInventoryInputLeft(&o);
      if (o.called638 != 1) { printf("FAIL call\n"); return 1; } }

    printf("PARITY_OK_0061a120\n");
    return 0;
}