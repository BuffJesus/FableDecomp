#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* s, int n) {
        // record what the ctor was called with
        g_last_ptr = s;
        g_last_n = n;
        m_data = (char*)s;
    }
    char* m_data;
    static const char* g_last_ptr;
    static int g_last_n;
};
const char* CCharString::g_last_ptr = 0;
int CCharString::g_last_n = 0;

struct CActionDoCreatureAction;

class CCombatSequence_BanditKingAttack {
public:
    virtual CCharString GetName() const;
    int filler;
};

CCharString CCombatSequence_BanditKingAttack::GetName() const
{
    return CCharString((const char*)0x1278868, -1);
}

int main()
{
    CCombatSequence_BanditKingAttack obj;
    CCharString r = obj.GetName();
    bool ok = (CCharString::g_last_ptr == (const char*)0x1278868)
           && (CCharString::g_last_n == -1)
           && (r.m_data == (char*)0x1278868);
    if (ok) printf("PASS_BANDITKINGATTACK_GETNAME\n");
    else    printf("FAIL ptr=%p n=%d\n", CCharString::g_last_ptr, CCharString::g_last_n);
    return ok ? 0 : 1;
}