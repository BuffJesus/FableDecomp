#include <cstdio>

struct SubThing {
    int lastArg;
    void Apply(int arg) { lastArg = arg; }
};

struct CBrainOutput {
    unsigned int flags;    // +0x00
    int pad4;              // +0x04
    SubThing sub;          // +0x08
    void SetAsPumpCombatActions(int arg);
};

void CBrainOutput::SetAsPumpCombatActions(int arg)
{
    this->sub.Apply(arg);
    this->flags |= 0x200;
}

int main()
{
    CBrainOutput b;
    b.flags = 0x1;
    b.pad4 = 0;
    b.sub.lastArg = -1;
    b.SetAsPumpCombatActions(77);

    bool ok = (b.flags == (0x1u | 0x200u)) && (b.sub.lastArg == 77);
    if (ok) {
        printf("PUMP_COMBAT_OK flags=%x arg=%d\n", b.flags, b.sub.lastArg);
        return 0;
    }
    printf("FAIL\n");
    return 1;
}