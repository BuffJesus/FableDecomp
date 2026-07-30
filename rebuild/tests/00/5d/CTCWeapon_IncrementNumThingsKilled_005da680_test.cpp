#include <stdio.h>

struct Thing {
    unsigned char pad_00[0x6c];
    unsigned char flags6c;
    unsigned char pad_6d[0x91 - 0x6d];
    unsigned char flags91;
};

struct CTCWeapon {
    unsigned char pad_00[0x1c];
    int numThingsKilled;
    void IncrementNumThingsKilled(Thing* thing);
};

void CTCWeapon::IncrementNumThingsKilled(Thing* thing)
{
    if (thing == 0) return;
    if (thing->flags91 & 0x1) return;
    if (!(thing->flags6c & 0x8)) return;
    ++this->numThingsKilled;
}

int main() {
    CTCWeapon w;
    w.numThingsKilled = 0;

    Thing t;
    t.flags6c = 0x08;   // eligible bit set
    t.flags91 = 0x00;   // not-eligible bit clear

    // valid kill -> increments
    w.IncrementNumThingsKilled(&t);
    if (w.numThingsKilled != 1) { printf("FAIL count=%d\n", w.numThingsKilled); return 1; }

    // flags91 bit0 set -> skip
    t.flags91 = 0x01;
    w.IncrementNumThingsKilled(&t);
    if (w.numThingsKilled != 1) { printf("FAIL flags91 skip\n"); return 1; }

    // flags6c bit3 clear -> skip
    t.flags91 = 0x00; t.flags6c = 0x00;
    w.IncrementNumThingsKilled(&t);
    if (w.numThingsKilled != 1) { printf("FAIL flags6c skip\n"); return 1; }

    // null -> no crash, no increment
    w.IncrementNumThingsKilled(0);
    if (w.numThingsKilled != 1) { printf("FAIL null\n"); return 1; }

    // another valid kill
    t.flags6c = 0x08; t.flags91 = 0x00;
    w.IncrementNumThingsKilled(&t);
    if (w.numThingsKilled != 2) { printf("FAIL count2=%d\n", w.numThingsKilled); return 1; }

    printf("OK_0x005da680 kills=%d\n", w.numThingsKilled);
    return 0;
}