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

class CCombatSequence_VillagerNearAttack {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_VillagerNearAttack::GetName() const
{
    return CCharString("VillagerNearAttack", -1);
}

int main()
{
    CCombatSequence_VillagerNearAttack obj;
    CCharString name = obj.GetName();
    if (strcmp(name.m_data, "VillagerNearAttack") == 0) {
        printf("GETNAME_OK\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}