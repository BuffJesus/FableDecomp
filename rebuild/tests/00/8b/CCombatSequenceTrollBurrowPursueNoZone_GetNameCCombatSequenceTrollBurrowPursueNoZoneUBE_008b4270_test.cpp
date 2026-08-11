#include <cstdio>

class CCharString {
public:
    CCharString(const char* s, int len);
    char* m_data;
};

// Local stand-in for the retail ctor at 0x99ebf0: record the args so the
// test can observe what GetName forwarded.
static const char* g_lastStr = 0;
static int g_lastLen = 0;

CCharString::CCharString(const char* s, int len) {
    g_lastStr = s;
    g_lastLen = len;
    m_data = (char*)s;
}

class CCombatSequence_TrollBurrowPursueNoZone {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollBurrowPursueNoZone::GetName() const {
    return CCharString((const char*)0x1278ac0, -1);
}

int main() {
    CCombatSequence_TrollBurrowPursueNoZone obj;
    CCharString r = obj.GetName();
    bool ok = (g_lastStr == (const char*)0x1278ac0)
           && (g_lastLen == -1)
           && (r.m_data == (char*)0x1278ac0);
    if (ok) {
        printf("GETNAME_TROLLBURROW_OK\n");
        return 0;
    }
    printf("GETNAME_TROLLBURROW_FAIL\n");
    return 1;
}