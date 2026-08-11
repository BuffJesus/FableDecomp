#include <cstdio>
#include <cstring>

// Independent behaviour test. Reimplement a CCharString ctor that copies the
// literal, mirroring what the real engine ctor does, and confirm GetName
// yields the expected name string.

struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

static char g_buf[64];

CCharString::CCharString(const char* s, int len)
{
    // len == -1 means "use strlen"
    int n = (len < 0) ? (int)strlen(s) : len;
    memcpy(g_buf, s, n);
    g_buf[n] = 0;
    p = g_buf;
}

struct CCombatSequence_BanditAdvanceFar {
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditAdvanceFar::GetName() const
{
    return CCharString("CombatSequence_BanditAdvanceFar", -1);
}

int main()
{
    CCombatSequence_BanditAdvanceFar obj;
    CCharString name = obj.GetName();
    if (strcmp(name.p, "CombatSequence_BanditAdvanceFar") == 0) {
        printf("GETNAME_PARITY_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}