#include <stdio.h>

struct SubObj1F8 {
    void Add(long &value);
};

struct CTCInventoryAbilities {
    char pad0[0x1f8];
    SubObj1F8 sub;
    void AddLearnedExpression(long value);
};

// captured state from the forwarded call
static long g_captured = 0;
static void *g_thisSeen = 0;

void SubObj1F8::Add(long &value)
{
    g_captured = value;
    g_thisSeen = (void*)this;
}

void CTCInventoryAbilities::AddLearnedExpression(long value)
{
    long local = value;
    sub.Add(local);
}

int main()
{
    CTCInventoryAbilities obj;
    obj.AddLearnedExpression(0x12345678);

    bool ok = true;
    // value forwarded by reference
    if (g_captured != 0x12345678) ok = false;
    // subobject 'this' must be base + 0x1f8
    void *expected = (char*)&obj + 0x1f8;
    if (g_thisSeen != expected) ok = false;

    if (ok) printf("EXPR_FWD_OK captured=%08lx off=0x1f8\n", g_captured);
    else    printf("FAIL captured=%08lx this=%p exp=%p\n", g_captured, g_thisSeen, expected);
    return ok ? 0 : 1;
}