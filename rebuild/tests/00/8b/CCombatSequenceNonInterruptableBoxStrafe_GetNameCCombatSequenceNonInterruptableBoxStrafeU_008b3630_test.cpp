#include <cstdio>
#include <cstring>
#include <cstdlib>

// ---- local standalone CCharString definition for the behaviour test ----
class CCharString {
public:
    CCharString(const char* s, int len) {
        int n = (len < 0) ? (int)strlen(s) : len;
        m_data = (char*)malloc(n + 1);
        memcpy(m_data, s, n);
        m_data[n] = 0;
    }
    char* m_data;
};

class CCombatSequence_NonInterruptableBoxStrafe {
public:
    virtual CCharString GetName() const;
};

static const char* const kName = "NonInterruptableBoxStrafe";

CCharString CCombatSequence_NonInterruptableBoxStrafe::GetName() const
{
    return CCharString(kName, -1);
}

int main()
{
    CCombatSequence_NonInterruptableBoxStrafe seq;
    CCharString s = seq.GetName();
    if (strcmp(s.m_data, "NonInterruptableBoxStrafe") == 0) {
        printf("GETNAME_OK\n");
        return 0;
    }
    printf("GETNAME_BAD:%s\n", s.m_data);
    return 1;
}