
#include <stdio.h>

struct PDef { void* vtbl; int refcount; };
struct Entry { int tag; PDef* pdef; };
struct CTCInventoryAbilities {
    char pad[0x160];
    Entry* table;
    PDef** GetAbilityPDef(PDef** out, int index);
};
PDef** CTCInventoryAbilities::GetAbilityPDef(PDef** out, int index)
{
    PDef* p = this->table[index].pdef;
    *out = p;
    if (p) p->refcount++;
    return out;
}

int main()
{
    // Build a small table with 3 entries.
    Entry tbl[3];
    PDef a; a.vtbl=(void*)0; a.refcount=5;
    PDef b; b.vtbl=(void*)0; b.refcount=100;
    tbl[0].tag=0; tbl[0].pdef=&a;
    tbl[1].tag=0; tbl[1].pdef=0;      // null pdef -> no inc
    tbl[2].tag=0; tbl[2].pdef=&b;

    CTCInventoryAbilities inv;
    inv.table = tbl;

    PDef* out = (PDef*)0xdeadbeef;

    // index 0 -> returns &a, refcount 5 -> 6
    PDef** r = inv.GetAbilityPDef(&out, 0);
    if (r != &out) { printf("FAIL r0\n"); return 1; }
    if (out != &a) { printf("FAIL out0\n"); return 1; }
    if (a.refcount != 6) { printf("FAIL rc0 %d\n", a.refcount); return 1; }

    // index 1 -> null, out=0, no crash
    out = (PDef*)0xdeadbeef;
    inv.GetAbilityPDef(&out, 1);
    if (out != 0) { printf("FAIL out1\n"); return 1; }

    // index 2 -> &b, refcount 100 -> 101
    inv.GetAbilityPDef(&out, 2);
    if (out != &b) { printf("FAIL out2\n"); return 1; }
    if (b.refcount != 101) { printf("FAIL rc2 %d\n", b.refcount); return 1; }

    printf("OK_0x005edfd9\n");
    return 0;
}