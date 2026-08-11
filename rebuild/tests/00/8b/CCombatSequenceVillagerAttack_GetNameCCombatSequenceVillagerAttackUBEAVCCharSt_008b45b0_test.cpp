#include <cstdio>
#include <cstring>

// Local stand-in for CCharString ctor (mimics a length-counted string).
class CCharString {
public:
    CCharString(const char* s, int n) {
        if (n < 0) n = (int)strlen(s);
        m_len = n;
        m_data = new char[n + 1];
        memcpy(m_data, s, n);
        m_data[n] = 0;
    }
    const char* c_str() const { return m_data; }
    int len() const { return m_len; }
private:
    char* m_data;
    int   m_len;
};

class CCombatSequence_VillagerAttack {
public:
    CCharString GetName() const;
};

CCharString CCombatSequence_VillagerAttack::GetName() const {
    return CCharString("VillagerAttack", -1);
}

int main() {
    CCombatSequence_VillagerAttack seq;
    CCharString n = seq.GetName();
    if (strcmp(n.c_str(), "VillagerAttack") == 0 && n.len() == 14) {
        printf("GETNAME_OK\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}