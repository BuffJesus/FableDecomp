#include <cstdio>

struct CTCInner
{
    char pad[0x25];
    char flag;      // +0x25
};

struct CTCTurncoat
{
    char pad0[0x14];
    CTCInner* inner;   // +0x14
    char pad1[0x2c - 0x18];
    char enabled;      // +0x2c

    int IsTurncoatable();
};

int main()
{
    CTCInner inner;
    CTCTurncoat obj;

    // both true -> 1
    obj.enabled = 1; obj.inner = &inner; inner.flag = 1;
    if (obj.IsTurncoatable() != 1) { printf("FAIL1\n"); return 1; }

    // enabled false -> 0 (inner not touched)
    obj.enabled = 0; inner.flag = 1;
    if (obj.IsTurncoatable() != 0) { printf("FAIL2\n"); return 1; }

    // enabled true, flag false -> 0
    obj.enabled = 1; inner.flag = 0;
    if (obj.IsTurncoatable() != 0) { printf("FAIL3\n"); return 1; }

    printf("TURNCOAT_OK\n");
    return 0;
}