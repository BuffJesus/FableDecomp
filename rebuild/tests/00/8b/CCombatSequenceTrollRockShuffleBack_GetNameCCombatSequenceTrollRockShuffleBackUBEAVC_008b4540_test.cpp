#include <stdio.h>
#include <string.h>

// Local stand-ins for the independent behaviour test.
class CCharString {
public:
    char* m_data;
    CCharString(const char* s, int len) {
        if (len < 0) len = (int)strlen(s);
        m_data = new char[len + 1];
        memcpy(m_data, s, len);
        m_data[len] = 0;
    }
};

class CCombatSequence_TrollRockShuffleBack {
public:
    virtual CCharString GetName() const;
};

static const char* g_testLit = "TrollRockShuffleBack";

CCharString CCombatSequence_TrollRockShuffleBack::GetName() const
{
    return CCharString(g_testLit, -1);
}

int main() {
    CCombatSequence_TrollRockShuffleBack obj;
    CCharString r = obj.GetName();
    if (strcmp(r.m_data, "TrollRockShuffleBack") == 0) {
        printf("GETNAME_PASS\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}