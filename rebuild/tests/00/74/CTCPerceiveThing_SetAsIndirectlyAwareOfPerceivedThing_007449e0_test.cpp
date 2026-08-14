#include <stdio.h>

struct Obj {
    unsigned char pad[0x2d];
    unsigned char flag2d;
    int sub_ret;   // what Sub_a01b50 returns for this obj
    int did;       // set when Do_744730 runs
};

static int Sub_a01b50(Obj* o) { return o->sub_ret; }
static void Do_744730(Obj* o) { o->did = 1; }

static void SetAware(Obj* self)
{
    if (Sub_a01b50(self) == 0)
        return;
    if (self->flag2d != 0)
        return;
    Do_744730(self);
}

int main()
{
    // case 1: sub_ret == 0 -> no action
    Obj a; a.flag2d = 0; a.sub_ret = 0; a.did = 0;
    SetAware(&a);

    // case 2: sub_ret != 0, flag2d != 0 -> no action
    Obj b; b.flag2d = 1; b.sub_ret = 5; b.did = 0;
    SetAware(&b);

    // case 3: sub_ret != 0, flag2d == 0 -> action
    Obj c; c.flag2d = 0; c.sub_ret = 5; c.did = 0;
    SetAware(&c);

    if (a.did == 0 && b.did == 0 && c.did == 1)
        printf("BEHAVIOUR_OK\n");
    else
        printf("BEHAVIOUR_FAIL a=%d b=%d c=%d\n", a.did, b.did, c.did);
    return 0;
}