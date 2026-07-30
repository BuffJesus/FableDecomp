#include <stdio.h>

struct Obj {
    char pad[0x17c];
    char flagA;      // +0x17c
    char pad2[0x184 - 0x17c - 1];
    char flagB;      // +0x184
};

Obj* g_obj = 0;   // ds:0x13b8ac4

bool CanAcceptChanges()
{
    Obj* p = g_obj;
    if (p == 0)
        return true;
    if (p->flagA == 0)
        return true;
    if (p->flagB == 0)
        return true;
    return false;
}

int main()
{
    // Case 1: null global -> true
    g_obj = 0;
    if (CanAcceptChanges() != true) { printf("FAIL null\n"); return 1; }

    static Obj obj;
    // Case 2: flagA==0 -> true
    obj.flagA = 0; obj.flagB = 1; g_obj = &obj;
    if (CanAcceptChanges() != true) { printf("FAIL flagA0\n"); return 1; }

    // Case 3: flagB==0 -> true
    obj.flagA = 1; obj.flagB = 0; g_obj = &obj;
    if (CanAcceptChanges() != true) { printf("FAIL flagB0\n"); return 1; }

    // Case 4: both non-zero -> false
    obj.flagA = 1; obj.flagB = 1; g_obj = &obj;
    if (CanAcceptChanges() != false) { printf("FAIL both\n"); return 1; }

    printf("OK_0x00556540\n");
    return 0;
}