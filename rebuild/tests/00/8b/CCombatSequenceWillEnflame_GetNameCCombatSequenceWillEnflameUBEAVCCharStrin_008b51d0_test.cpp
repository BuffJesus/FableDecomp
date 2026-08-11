#include <cstdio>
#include <cstring>

// Local standalone model of CCharString for the behaviour test.
class CCharString {
public:
    CCharString(const char* s, int n) {
        if (n < 0) n = (int)strlen(s);
        m_p = new char[n + 1];
        memcpy(m_p, s, n);
        m_p[n] = 0;
    }
    const char* c_str() const { return m_p; }
private:
    char* m_p;
};

class CCombatSequence_WillEnflame {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_WillEnflame::GetName() const
{
    return CCharString("SEQ_HERO_WILL_ENFLAME", -1);
}

int main() {
    CCombatSequence_WillEnflame o;
    CCharString r = o.GetName();
    if (strcmp(r.c_str(), "SEQ_HERO_WILL_ENFLAME") == 0)
        printf("PASS_WILLENFLAME_GETNAME\n");
    else
        printf("FAIL\n");
    return strcmp(r.c_str(), "SEQ_HERO_WILL_ENFLAME") == 0 ? 0 : 1;
}