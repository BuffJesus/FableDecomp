#include <stdio.h>

struct CTCTavernGame {
    char pad0[0x34];
    int  field_34;   // 0x34
    char pad1[0x08];
    int  field_40;   // 0x40
    char pad2[0x5c];
    char field_a0;   // 0xa0
    char field_a1;   // 0xa1
    char field_a2;   // 0xa2
    char pad3[0x10];
};

void __fastcall NewGame(CTCTavernGame *this_)
{
    this_->field_40 -= this_->field_34;
    this_->field_a0 = 0;
    this_->field_a2 = 0;
}

int main()
{
    CTCTavernGame g;
    g.field_34 = 100;
    g.field_40 = 250;
    g.field_a0 = (char)0x7F;
    g.field_a1 = (char)0x55;   // must be untouched
    g.field_a2 = (char)0x7F;

    CTCTavernGame *p = &g;
    __asm {
        mov ecx, p
        call NewGame
    }

    int ok = 1;
    if (g.field_40 != 150) ok = 0;          // 250 - 100
    if (g.field_a0 != 0)   ok = 0;
    if (g.field_a2 != 0)   ok = 0;
    if (g.field_a1 != (char)0x55) ok = 0;   // untouched
    if (g.field_34 != 100) ok = 0;          // untouched

    if (ok) printf("OK_0x005e9a70\n");
    else    printf("BAD field40=%d a0=%d a1=%d a2=%d\n", g.field_40, g.field_a0, g.field_a1, g.field_a2);
    return 0;
}