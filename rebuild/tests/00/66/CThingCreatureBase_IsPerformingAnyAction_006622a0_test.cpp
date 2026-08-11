#include <cstdio>

struct ActionState {
    char pad[0x61];
    char busyFlag;
};

struct CThingCreatureBase {
    char pad[0x100];
    ActionState* action;
    int IsPerformingAnyAction();
};

int main()
{
    CThingCreatureBase c;
    ActionState a;

    // null action -> 0
    c.action = 0;
    int r1 = c.IsPerformingAnyAction();

    // action present, busy flag 0 -> 1
    a.busyFlag = 0;
    c.action = &a;
    int r2 = c.IsPerformingAnyAction();

    // action present, busy flag nonzero -> 0
    a.busyFlag = 7;
    c.action = &a;
    int r3 = c.IsPerformingAnyAction();

    if (!r1 && r2 && !r3) {
        printf("ACTION_OK\n");
        return 0;
    }
    printf("ACTION_FAIL %d %d %d\n", r1, r2, r3);
    return 1;
}