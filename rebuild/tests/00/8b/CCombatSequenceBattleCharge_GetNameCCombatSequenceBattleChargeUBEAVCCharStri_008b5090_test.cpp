#include <cstdio>
#include <cstring>

class CCharString {
public:
    void Init(const char* s, int len) {
        if (len < 0) len = (int)strlen(s);
        m_len = len;
        m_data = new char[len + 1];
        memcpy(m_data, s, len);
        m_data[len] = 0;
    }
    char* m_data;
    int m_len;
};

static const char* const s_name = "CombatSequence_BattleCharge";

struct CCombatSequence_BattleCharge {
    void* vtbl;
    CCharString* GetName_CCombatSequence_BattleCharge(CCharString* __ret) const;
};

CCharString* CCombatSequence_BattleCharge::GetName_CCombatSequence_BattleCharge(CCharString* __ret) const {
    __ret->Init(s_name, -1);
    return __ret;
}

int main() {
    CCombatSequence_BattleCharge obj;
    obj.vtbl = 0;
    char buf[sizeof(CCharString)];
    CCharString* r = obj.GetName_CCombatSequence_BattleCharge((CCharString*)buf);
    if (r == (CCharString*)buf && strcmp(r->m_data, "CombatSequence_BattleCharge") == 0
        && r->m_len == 27) {
        printf("GETNAME_BATTLECHARGE_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}