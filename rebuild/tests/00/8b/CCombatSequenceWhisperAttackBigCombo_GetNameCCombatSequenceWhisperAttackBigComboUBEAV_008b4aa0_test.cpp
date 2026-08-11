#include <cstdio>

// Standalone behaviour test. Reimplement CCharString locally so we can observe
// that GetName constructs from the fixed literal pointer with length -1.

static const char* g_last_str = 0;
static int         g_last_len = 999;

struct CCharString {
    char* m_data;
    CCharString() { m_data = 0; }
    CCharString(const char* s, int n) { g_last_str = s; g_last_len = n; m_data = (char*)s; }
    CCharString(const CCharString& o) { m_data = o.m_data; }
    ~CCharString() {}
};

struct CCombatSequenceWhisperAttackBigCombo {
    virtual CCharString GetName();
};

CCharString CCombatSequenceWhisperAttackBigCombo::GetName()
{
    return CCharString((const char*)0x1278d20, -1);
}

int main()
{
    CCombatSequenceWhisperAttackBigCombo obj;
    obj.GetName();

    bool ok = (g_last_str == (const char*)0x1278d20) && (g_last_len == -1);
    if (ok) {
        printf("GETNAME_BIGCOMBO_OK ptr=0x1278d20 len=-1\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}