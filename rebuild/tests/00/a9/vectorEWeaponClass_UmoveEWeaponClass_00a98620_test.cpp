#include <stdio.h>

typedef int EWeaponClass;
struct Pair { int a; int b; };

// Behaviour model of the callee: it receives p and a zero-initialized pair by address,
// and (for this test) returns p while asserting the pair was zeroed.
static EWeaponClass* g_seen_q_a = 0;
static int g_q_a = 7, g_q_b = 9;

static EWeaponClass* model_helper(EWeaponClass* p, Pair* q)
{
    g_q_a = q->a;
    g_q_b = q->b;
    return p;
}

static EWeaponClass* model_Umove(EWeaponClass* p)
{
    Pair local;
    local.a = 0;
    local.b = 0;
    return model_helper(p, &local);
}

int main()
{
    EWeaponClass w = 123;
    EWeaponClass* r = model_Umove(&w);
    if (r == &w && g_q_a == 0 && g_q_b == 0) {
        printf("UMOVE_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}