#include <cstdio>

struct Pair { unsigned int a; unsigned int b; };

bool __fastcall GetAssignedInputsPassive(Pair* self, Pair* other)
{
    if (self->b == other->b)
        return self->a < other->a;
    return self->b < other->b;
}

int main()
{
    Pair p1 = {5, 10};
    Pair p2 = {7, 10};
    Pair p3 = {5, 11};
    Pair p4 = {5, 10};

    bool ok = true;
    // equal high, lower low -> true
    if (GetAssignedInputsPassive(&p1, &p2) != true) ok = false;
    // equal high, higher low -> false
    if (GetAssignedInputsPassive(&p2, &p1) != false) ok = false;
    // lower high -> true regardless of low
    if (GetAssignedInputsPassive(&p1, &p3) != true) ok = false;
    // higher high -> false
    if (GetAssignedInputsPassive(&p3, &p1) != false) ok = false;
    // fully equal -> false (not less)
    if (GetAssignedInputsPassive(&p1, &p4) != false) ok = false;

    if (ok) printf("OK_00c38380\n");
    else    printf("FAIL_00c38380\n");
    return ok ? 0 : 1;
}