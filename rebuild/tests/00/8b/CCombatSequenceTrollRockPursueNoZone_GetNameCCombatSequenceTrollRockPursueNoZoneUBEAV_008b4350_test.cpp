#include <stdio.h>
#include <string.h>

// Local standalone stand-in for CCharString (test-only).
class CCharString {
public:
    CCharString(const char* s, int len) {
        int n = (len < 0) ? (int)strlen(s) : len;
        m_len = n;
        m_pData = new char[n + 1];
        memcpy(m_pData, s, n);
        m_pData[n] = 0;
    }
    const char* c_str() const { return m_pData; }
    int len() const { return m_len; }
private:
    char* m_pData;
    int m_len;
};

class CCombatSequence_TrollRockPursueNoZone {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollRockPursueNoZone::GetName() const {
    return CCharString("CCombatSequence_TrollRockPursueNoZone", -1);
}

int main() {
    CCombatSequence_TrollRockPursueNoZone obj;
    CCharString n = obj.GetName();
    if (strcmp(n.c_str(), "CCombatSequence_TrollRockPursueNoZone") == 0 &&
        n.len() == (int)strlen("CCombatSequence_TrollRockPursueNoZone")) {
        printf("GETNAME_OK\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}