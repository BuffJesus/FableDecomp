#include <cstdio>
#include <cstring>

// Independent behaviour test.
class CCharString
{
public:
    CCharString(const char* s, int len);
    const char* data() const { return m_data; }
private:
    char* m_data;
};

CCharString::CCharString(const char* s, int len)
{
    m_data = 0;
    if (s && len != 0)
        m_data = const_cast<char*>(s);
}

class CCombatSequence_BanditBoast
{
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditBoast::GetName() const
{
    return CCharString("SEQUENCE_BANDIT_BOAST", -1);
}

int main()
{
    CCombatSequence_BanditBoast obj;
    CCharString name = obj.GetName();
    if (name.data() && std::strcmp(name.data(), "SEQUENCE_BANDIT_BOAST") == 0)
        std::printf("BANDITBOAST_NAME_OK\n");
    else
        std::printf("FAIL\n");
    return (name.data() && std::strcmp(name.data(), "SEQUENCE_BANDIT_BOAST") == 0) ? 0 : 1;
}