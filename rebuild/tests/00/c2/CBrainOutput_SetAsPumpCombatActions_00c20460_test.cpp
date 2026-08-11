#include <stdio.h>

struct Member {
    void DoPump(int arg);
};

struct CBrainOutput {
    unsigned int flags;
    int          pad4;
    Member       member;

    void SetAsPumpCombatActions(int arg);
};

static int g_pumped = -1;
static Member* g_this = 0;

void Member::DoPump(int arg) {
    g_pumped = arg;
    g_this = this;
}

void CBrainOutput::SetAsPumpCombatActions(int arg) {
    this->member.DoPump(arg);
    this->flags |= 0x80;
}

int main() {
    CBrainOutput obj;
    obj.flags = 0x01;
    obj.pad4  = 0;
    g_pumped  = -1;
    g_this    = 0;

    obj.SetAsPumpCombatActions(1234);

    bool ok = true;
    if (g_pumped != 1234) ok = false;                       // arg forwarded
    if (g_this != &obj.member) ok = false;                  // called on member at +8
    if (obj.flags != (0x01 | 0x80)) ok = false;             // OR-in 0x80, preserves existing bits

    // second call preserves the already-set bit
    obj.flags = 0x80;
    obj.SetAsPumpCombatActions(7);
    if (g_pumped != 7) ok = false;
    if (obj.flags != 0x80) ok = false;

    printf(ok ? "PUMP_COMBAT_OK\n" : "PUMP_COMBAT_FAIL\n");
    return ok ? 0 : 1;
}