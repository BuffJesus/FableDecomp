#include <cstdio>
#include <cstring>
#include <cstdlib>

class CCharString {
public:
    CCharString(const char* s, int len);
    char* m_data;
};

CCharString::CCharString(const char* s, int len)
{
    int n = (len < 0) ? (int)strlen(s) : len;
    m_data = (char*)malloc(n + 1);
    memcpy(m_data, s, n);
    m_data[n] = 0;
}

class CCombatSequence_FodderMainAttack {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_FodderMainAttack::GetName() const
{
    return CCharString("FodderMainAttack", -1);
}

int main()
{
    CCombatSequence_FodderMainAttack obj;
    CCharString name = obj.GetName();
    if (strcmp(name.m_data, "FodderMainAttack") == 0) {
        printf("GETNAME_OK\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}