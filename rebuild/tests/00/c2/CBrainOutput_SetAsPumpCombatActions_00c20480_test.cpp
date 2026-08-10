#include <stdio.h>

struct SubObj {
    long lastArg;
    int called;
    void method(long a);
};

struct CBrainOutput {
    unsigned long flags; // +0
    long pad4;           // +4
    SubObj sub;          // +8
    void SetAsPumpCombatActions(long a);
};

void SubObj::method(long a)
{
    this->lastArg = a;
    this->called = 1;
}

void CBrainOutput::SetAsPumpCombatActions(long a)
{
    this->sub.method(a);
    this->flags |= 4;
}

int main()
{
    CBrainOutput o;
    o.flags = 1;      // bit0 set to prove OR (not assign): expect 1|4 = 5
    o.pad4 = 0;
    o.sub.lastArg = -999;
    o.sub.called = 0;

    o.SetAsPumpCombatActions(77);

    if (o.sub.called == 1 && o.sub.lastArg == 77 && o.flags == 5) {
        printf("00c20480_TEST PASS\n");
    } else {
        printf("FAIL called=%d arg=%ld flags=%lu\n", o.sub.called, o.sub.lastArg, o.flags);
    }
    return 0;
}