#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CCharString {
    char* m_buf;
    CCharString(const char* s, int n);
};

// Local stand-in for CCharString::CCharString(char const*, int) @ 0x0099ebf0.
// n==-1 means "use strlen". Copies the literal.
CCharString::CCharString(const char* s, int n)
{
    int len = (n < 0) ? (int)strlen(s) : n;
    m_buf = (char*)malloc(len + 1);
    memcpy(m_buf, s, len);
    m_buf[len] = 0;
}

struct CCombatSequenceWhisperStrafeAround {
    void* vtbl;
    CCharString GetName() const;
};

CCharString CCombatSequenceWhisperStrafeAround::GetName() const
{
    return CCharString("CombatSequenceWhisperStrafeAround", -1);
}

int main()
{
    CCombatSequenceWhisperStrafeAround obj;
    obj.vtbl = 0;
    CCharString r = obj.GetName();
    if (r.m_buf && strcmp(r.m_buf, "CombatSequenceWhisperStrafeAround") == 0) {
        printf("GETNAME_OK\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}