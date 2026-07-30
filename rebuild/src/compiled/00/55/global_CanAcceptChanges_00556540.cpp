// CanAcceptChanges @ 0x00556540
struct Obj {
    char pad[0x17c];
    char flagA;      // +0x17c
    char pad2[0x184 - 0x17c - 1];
    char flagB;      // +0x184
};

extern Obj* g_obj;   // ds:0x13b8ac4

bool CanAcceptChanges()
{
    Obj* p = g_obj;
    if (p != 0 && p->flagA != 0 && p->flagB != 0)
        return false;
    return true;
}