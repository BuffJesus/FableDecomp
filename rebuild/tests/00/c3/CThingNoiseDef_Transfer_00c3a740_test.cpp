#include <cstdio>

struct CPersistContext;
struct CThingNoiseDef;

static int g_base_called = 0;
static int g_sub_a_ok = 0;
static int g_sub_b = 0;
static int g_sub_self_off = -1;

struct SubObj {
    char pad[4];
    void M(CThingNoiseDef* a, int b);
};
struct CThingNoiseDef {
    char pad[0x14];
    SubObj sub;
};

void SubObj::M(CThingNoiseDef* a, int b) {
    g_sub_a_ok = (reinterpret_cast<char*>(this) == reinterpret_cast<char*>(a) + 0x14);
    g_sub_b = b;
    g_sub_self_off = (int)(reinterpret_cast<char*>(this) - reinterpret_cast<char*>(a));
}

void __fastcall BaseChain(CThingNoiseDef* self) { g_base_called = 1; }

CThingNoiseDef* __fastcall Transfer(CThingNoiseDef* self, CPersistContext* ctx);

int main() {
    CThingNoiseDef obj;
    CThingNoiseDef* r = Transfer(&obj, 0);
    if (r == &obj && g_base_called && g_sub_a_ok && g_sub_b == 1 && g_sub_self_off == 0x14)
        printf("00c3a740_TEST PASS\n");
    else
        printf("FAIL base=%d a=%d b=%d off=%d r=%d\n", g_base_called, g_sub_a_ok, g_sub_b, g_sub_self_off, (int)(r==&obj));
    return 0;
}