#include <cstdio>

struct SubObj {
    long got;
    int called;
    void call(long a);
};

void SubObj::call(long a) {
    this->got = a;
    this->called = 1;
}

struct CBrainOutput {
    unsigned long flags;   // +0
    char pad[4];           // +4
    SubObj sub;            // +8
};

void __fastcall SetAsPumpCombatActions(CBrainOutput* self, int /*edx*/, long a);

int main() {
    CBrainOutput o;
    o.flags = 1;
    o.sub.got = 0;
    o.sub.called = 0;
    SetAsPumpCombatActions(&o, 0, 12345);
    if (o.sub.called == 1 && o.sub.got == 12345 && o.flags == (1u | 8u)) {
        std::printf("00c204a0_TEST PASS\n");
    } else {
        std::printf("FAIL flags=%lu got=%ld called=%d\n", o.flags, o.sub.got, o.sub.called);
    }
    return 0;
}