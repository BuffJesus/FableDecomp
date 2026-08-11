#include <cstdio>

struct Sub {
    int calls;
    int retval;
    int M();
};

// controlled behaviour: first call returns retval, decrement so second returns 0
int Sub::M() {
    calls++;
    int r = retval;
    if (retval) retval = 0;
    return r;
}

struct CCreatureAction_PickCrop {
    char pad[0xa8];
    Sub sub;
    void PerformAction();
};

void CCreatureAction_PickCrop::PerformAction()
{
    if (this->sub.M())
        this->sub.M();
}

int main()
{
    // Case 1: first M returns nonzero -> two calls total
    CCreatureAction_PickCrop a;
    a.sub.calls = 0;
    a.sub.retval = 1;
    a.PerformAction();
    bool ok1 = (a.sub.calls == 2);

    // Case 2: first M returns zero -> one call total
    CCreatureAction_PickCrop b;
    b.sub.calls = 0;
    b.sub.retval = 0;
    b.PerformAction();
    bool ok2 = (b.sub.calls == 1);

    if (ok1 && ok2) {
        printf("PICKCROP_OK calls=%d,%d\n", a.sub.calls, b.sub.calls);
        return 0;
    }
    printf("PICKCROP_FAIL calls=%d,%d\n", a.sub.calls, b.sub.calls);
    return 1;
}