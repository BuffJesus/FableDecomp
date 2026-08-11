#include <cstdio>

static int g_seq[8];
static int g_n = 0;
static void* g_last_self = 0;

extern void __fastcall helperA(void* self);
extern void __fastcall helperB(void* self);

void __fastcall helperA(void* self) { g_seq[g_n++] = 1; g_last_self = self; }
void __fastcall helperB(void* self) { g_seq[g_n++] = 2; g_last_self = self; }

void __fastcall _Dest_val_pair_CCharString_CSymbolInfo(void* self, void* edx);

int main()
{
    int obj = 0;
    void* self = &obj;
    _Dest_val_pair_CCharString_CSymbolInfo(self, 0);

    bool ok = (g_n == 3) &&
              (g_seq[0] == 1) && (g_seq[1] == 2) && (g_seq[2] == 1) &&
              (g_last_self == self);
    if (ok) printf("00c4b040_TEST PASS\n");
    else    printf("00c4b040_TEST FAIL n=%d %d %d %d\n", g_n, g_seq[0], g_seq[1], g_seq[2]);
    return 0;
}