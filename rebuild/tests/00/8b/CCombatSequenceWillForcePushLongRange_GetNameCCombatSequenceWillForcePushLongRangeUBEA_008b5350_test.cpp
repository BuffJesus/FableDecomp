#include <stdio.h>
#include <string.h>

// Standalone behaviour test for CCombatSequence_WillForcePushLongRange::GetName.
// We provide a local CCharString whose (const char*, int) ctor records the string,
// so we can observe which literal GetName constructs.

static char g_last[256];
static int  g_last_n;

class CCharString {
public:
    CCharString(const char* s, int n) {
        g_last_n = n;
        if (s) { strncpy(g_last, s, sizeof(g_last)-1); g_last[sizeof(g_last)-1]=0; }
        else   { g_last[0]=0; }
        m_p = g_last;
    }
    const char* c_str() const { return m_p; }
private:
    char* m_p;
};

class CActionDoCreatureAction;

class CCombatSequence_WillForcePushLongRange {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_WillForcePushLongRange::GetName() const
{
    return CCharString("WillForcePushLongRange", -1);
}

int main()
{
    CCombatSequence_WillForcePushLongRange obj;
    CCharString r = obj.GetName();
    if (strcmp(r.c_str(), "WillForcePushLongRange") == 0 && g_last_n == -1) {
        printf("GETNAME_OK\n");
        return 0;
    }
    printf("GETNAME_BAD:%s:%d\n", r.c_str(), g_last_n);
    return 1;
}