#include <stdio.h>

struct CTCLook {
    char pad[0x1dc];
};

void __fastcall SetHeadTurnState(CTCLook* self, int edx_unused, long state)
{
    long* pState = (long*)((char*)self + 0x19c);
    if (*pState != state) {
        *pState = state;
        *(unsigned long*)((char*)self + 0x1d8) |= 0x400000;
    }
}

int main()
{
    char buf[0x1dc];
    CTCLook* o = (CTCLook*)buf;
    long* pState = (long*)(buf + 0x19c);
    unsigned long* pFlag = (unsigned long*)(buf + 0x1d8);

    // case 1: change value -> stores + sets flag
    *pState = 5;
    *pFlag = 0;
    SetHeadTurnState(o, 0, 9);
    if (*pState != 9) { printf("FAIL1\n"); return 1; }
    if (*pFlag != 0x400000) { printf("FAIL2\n"); return 1; }

    // case 2: same value -> no store, no flag change
    *pState = 9;
    *pFlag = 0x11;
    SetHeadTurnState(o, 0, 9);
    if (*pState != 9) { printf("FAIL3\n"); return 1; }
    if (*pFlag != 0x11) { printf("FAIL4\n"); return 1; }

    // case 3: change with existing flag bits -> OR preserves
    *pState = 1;
    *pFlag = 0x1;
    SetHeadTurnState(o, 0, 2);
    if (*pState != 2) { printf("FAIL5\n"); return 1; }
    if (*pFlag != 0x400001) { printf("FAIL6\n"); return 1; }

    printf("BEHAVIOUR_OK\n");
    return 0;
}