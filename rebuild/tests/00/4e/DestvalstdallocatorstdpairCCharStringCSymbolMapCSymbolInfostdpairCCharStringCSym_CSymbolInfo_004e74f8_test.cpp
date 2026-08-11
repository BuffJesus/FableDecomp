#include <stdio.h>

struct HeadStr { char pad[0x0c]; ~HeadStr(); };
struct MidObj  { char pad[0x18]; ~MidObj();  };
struct TailObj { char pad[4];    ~TailObj(); };
struct PairType { HeadStr first; MidObj mid; TailObj tail; };

void __fastcall _Dest_val_CSymbolInfo___(PairType *self);

static int g_order[3];
static int g_n = 0;
HeadStr::~HeadStr() { g_order[g_n++] = 3; }
MidObj::~MidObj()   { g_order[g_n++] = 2; }
TailObj::~TailObj() { g_order[g_n++] = 1; }

int main()
{
    // Verify offsets match the disasm expectations.
    PairType p;
    if (sizeof(HeadStr) != 0x0c) { printf("FAIL size head\n"); return 1; }
    if (sizeof(PairType) < 0x28) { printf("FAIL size pair %d\n", (int)sizeof(PairType)); return 1; }

    _Dest_val_CSymbolInfo___(&p);

    // Expected destroy order: tail(1), mid(2), head(3).
    if (g_n == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3) {
        printf("DESTVAL_OK order=%d%d%d\n", g_order[0], g_order[1], g_order[2]);
        return 0;
    }
    printf("FAIL order n=%d %d %d %d\n", g_n, g_order[0], g_order[1], g_order[2]);
    return 1;
}