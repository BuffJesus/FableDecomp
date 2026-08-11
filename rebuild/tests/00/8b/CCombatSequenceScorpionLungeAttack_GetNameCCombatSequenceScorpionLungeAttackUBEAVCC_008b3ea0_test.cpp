#include <cstdio>
#include <cstring>

struct CCharString {
    char* m_data;
    CCharString(const char* s, long n);
};

const char kScorpionLungeName[] = "SCORPION_LUNGE_ATTACK";

// Mimic CCharString(const char*, long): with n == -1, take strlen and copy.
CCharString::CCharString(const char* s, long n)
{
    long len = (n < 0) ? (long)std::strlen(s) : n;
    m_data = new char[len + 1];
    std::memcpy(m_data, s, len);
    m_data[len] = '\0';
}

struct CCombatSequence_ScorpionLungeAttack {
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_ScorpionLungeAttack::GetName() const
{
    return CCharString(kScorpionLungeName, -1);
}

int main()
{
    CCombatSequence_ScorpionLungeAttack obj;
    CCharString name = obj.GetName();
    if (name.m_data && std::strcmp(name.m_data, "SCORPION_LUNGE_ATTACK") == 0) {
        std::printf("GETNAME_OK\n");
        return 0;
    }
    std::printf("GETNAME_FAIL\n");
    return 1;
}