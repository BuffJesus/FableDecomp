#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* s, long n);
    const char* m_data;
};

// local definition of the CCharString(const char*, long) ctor for the test
CCharString::CCharString(const char* s, long n)
{
    (void)n;
    m_data = s;
}

class CCombatSequence_TrollRockIdle {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollRockIdle::GetName() const
{
    return CCharString((const char*)0x1278b88, -1);
}

int main()
{
    CCombatSequence_TrollRockIdle obj;
    CCharString r = obj.GetName();
    if (r.m_data == (const char*)0x1278b88) {
        printf("TROLLROCKIDLE_GETNAME_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}