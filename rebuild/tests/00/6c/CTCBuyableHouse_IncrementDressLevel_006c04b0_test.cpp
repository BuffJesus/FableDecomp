#include <stdio.h>

struct CTCBuyableHouse {
    char pad[0x1c];
    int field_1c;
    char pad2[0x14];
    int field_34;
    void SetDressLevel(int lvl);
};

static int g_lastLvl = -999;
void CTCBuyableHouse::SetDressLevel(int lvl) { g_lastLvl = lvl; }
int GetNewThing() { return 0x1234; }

void __fastcall CTCBuyableHouse_IncrementDressLevel(CTCBuyableHouse* self);

int main() {
    CTCBuyableHouse h;
    h.field_1c = 41;
    h.field_34 = 0;
    CTCBuyableHouse_IncrementDressLevel(&h);
    if (g_lastLvl == 42 && h.field_34 == 0x1234) {
        printf("OK_006c04b0\n");
    } else {
        printf("FAIL lvl=%d f34=%x\n", g_lastLvl, h.field_34);
    }
    return 0;
}