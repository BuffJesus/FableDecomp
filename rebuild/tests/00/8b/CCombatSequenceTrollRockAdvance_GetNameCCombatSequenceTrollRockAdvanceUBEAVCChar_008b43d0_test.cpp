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

class CCombatSequence_TrollRockAdvance
{
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollRockAdvance::GetName() const
{
    return CCharString("SEQUENCE_TROLL_ROCK_ADVANCE", -1);
}

int main()
{
    CCombatSequence_TrollRockAdvance obj;
    CCharString name = obj.GetName();
    if (name.data() && std::strcmp(name.data(), "SEQUENCE_TROLL_ROCK_ADVANCE") == 0)
        std::printf("TROLLROCKADVANCE_NAME_OK\n");
    else
        std::printf("FAIL\n");
    return (name.data() && std::strcmp(name.data(), "SEQUENCE_TROLL_ROCK_ADVANCE") == 0) ? 0 : 1;
}