#include <cstdio>
#include <cstring>

class CCharString
{
public:
    CCharString(const char* s, int n);
    char buf[64];
    int len;
};

CCharString::CCharString(const char* s, int n)
{
    std::strncpy(buf, s, 63);
    buf[63] = 0;
    len = n;
}

class CCombatSequence_TrollDoubleSwipe
{
public:
    virtual CCharString GetName() const;
};

int main()
{
    CCombatSequence_TrollDoubleSwipe obj;
    CCharString r = obj.GetName();
    if (std::strcmp(r.buf, "TrollDoubleSwipe") == 0 && r.len == -1)
    {
        std::printf("PASS_TROLLDOUBLESWIPE_GETNAME\n");
        return 0;
    }
    std::printf("FAIL got=%s len=%d\n", r.buf, r.len);
    return 1;
}