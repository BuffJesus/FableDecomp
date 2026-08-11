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
    g_last_self = this;
    g_last_s = s;
    g_last_n = n;
    this->m_data = (char*)s;
}

struct CActionDoCreatureAction;

struct CCombatSequence_BanditAttackSide {
    void* vtbl;
    CCharString* GetName(CCharString* result);
};

int main()
{
    CCombatSequence_BanditAttackSide obj;
    obj.vtbl = 0;
    CCharString out;
    out.m_data = 0;

    CCharString* r = obj.GetName(&out);

    int ok = 1;
    if (r != &out) ok = 0;
    if (g_last_self != &out) ok = 0;
    if (g_last_s != (const char*)0x012787b8) ok = 0;
    if (g_last_n != -1) ok = 0;

    if (ok) printf("GETNAME_BANDITATTACKSIDE_OK\n");
    else printf("FAIL s=%p n=%d self=%p r=%p\n", g_last_s, g_last_n, g_last_self, r);
    return ok ? 0 : 1;
}