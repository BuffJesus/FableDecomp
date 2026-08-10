#include <cstdio>
struct Inner {
    char pad[0x58];
    char flag;
};
struct CTCQuestCard {
    char pad0[0xc];
    Inner* inner;
    char pad1[4];
    int  field14;
};

bool __fastcall IsCore(CTCQuestCard* self)
{
    if (self->field14 <= -1) {
        if (self->inner->flag)
            return true;
    }
    return false;
}

int main()
{
    Inner innr; innr.flag = 1;
    Inner* pin = &innr;
    CTCQuestCard c; c.inner = pin;
    CTCQuestCard* pc = &c;

    c.field14 = 0;   if (IsCore(pc)) { printf("FAIL1\n"); return 1; }
    c.field14 = 5;   if (IsCore(pc)) { printf("FAIL2\n"); return 1; }

    c.field14 = -1;  if (!IsCore(pc)) { printf("FAIL3\n"); return 1; }
    c.field14 = -100;if (!IsCore(pc)) { printf("FAIL4\n"); return 1; }

    innr.flag = 0;
    c.field14 = -1;  if (IsCore(pc)) { printf("FAIL5\n"); return 1; }

    printf("OK_00702280\n");
    return 0;
}