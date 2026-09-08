#include "engine/CCombatSequence_BanditAttackSide.h"
#include <stdio.h>

struct CCharString {
    char* m_data;
    void Init(const char* s, int n);
};

static const char* g_last_s;
static int g_last_n;
static CCharString* g_last_self;

void CCharString::Init(const char* s, int n)
{
    g_last_self = this; g_last_s = s; g_last_n = n; m_data = (char*)s;
}

struct CCombatSequence_BanditAttackSide_Methods : CCombatSequence_BanditAttackSide {
    CCharString* GetName(CCharString* result);
};

int main()
{
    CCombatSequence_BanditAttackSide_Methods obj;
    obj.__vftable = 0;
    CCharString out; out.m_data = 0;
    CCharString* r = obj.GetName(&out);
    int ok = r == &out && g_last_self == &out &&
             g_last_s == (const char*)0x012787b8 && g_last_n == -1;
    if (ok) printf("GETNAME_BANDITATTACKSIDE_OK\n");
    else printf("FAIL s=%p n=%d self=%p r=%p\n", g_last_s, g_last_n, g_last_self, r);
    return ok ? 0 : 1;
}
