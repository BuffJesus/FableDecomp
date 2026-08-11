#include <cstdio>
#include <cstring>

class CCharString {
public:
    char* m_ptr;
    CCharString(const char* s, int len);
};

// standalone impl of the extern ctor for linking the test
CCharString::CCharString(const char* s, int len)
{
    static char buf[256];
    int n = (len < 0) ? (int)std::strlen(s) : len;
    if (n > 255) n = 255;
    std::memcpy(buf, s, n);
    buf[n] = 0;
    m_ptr = buf;
}

class CCombatSequence_WillLightning {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_WillLightning::GetName() const
{
    return CCharString("CombatSequence_WillLightning", -1);
}

int main()
{
    CCombatSequence_WillLightning obj;
    CCharString r = obj.GetName();
    if (r.m_ptr && std::strcmp(r.m_ptr, "CombatSequence_WillLightning") == 0)
        std::printf("GETNAME_OK\n");
    else
        std::printf("GETNAME_FAIL\n");
    return 0;
}