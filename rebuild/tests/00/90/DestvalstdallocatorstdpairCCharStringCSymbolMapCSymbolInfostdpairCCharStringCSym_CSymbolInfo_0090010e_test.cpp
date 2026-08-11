#include <stdio.h>

// Independent behaviour test for the _Dest_val pair destroyer.
// Verifies the destroy order: second_name1 (+0x1c), second_name0 (+0x14),
// first (+0).  Member dtors record the byte offset of 'this' from a global base.

static int g_order[8];
static int g_n = 0;
static char *g_base = 0;

class CCharString
{
public:
    void Destroy();
    char *m_pData;
};

class CFirstMember
{
public:
    void Destroy();
    char *m_pData;
};

struct SymPair
{
    CFirstMember first;
    int   pad0;
    int   pad1;
    int   pad2;
    int   pad3;
    CCharString second_name0;
    int   pad4;
    CCharString second_name1;
};

void __fastcall Dest_val_SymPair(SymPair *self);

void CCharString::Destroy()
{
    g_order[g_n++] = (int)((char*)this - g_base);
}

void CFirstMember::Destroy()
{
    g_order[g_n++] = (int)((char*)this - g_base);
}

int main()
{
    SymPair p;
    g_base = (char*)&p;
    g_n = 0;

    Dest_val_SymPair(&p);

    if (g_n != 3) { printf("FAIL count=%d\n", g_n); return 1; }
    if (g_order[0] != 0x1c) { printf("FAIL o0=%x\n", g_order[0]); return 1; }
    if (g_order[1] != 0x14) { printf("FAIL o1=%x\n", g_order[1]); return 1; }
    if (g_order[2] != 0x00) { printf("FAIL o2=%x\n", g_order[2]); return 1; }

    printf("DESTROY_ORDER_OK 1c 14 00\n");
    return 0;
}