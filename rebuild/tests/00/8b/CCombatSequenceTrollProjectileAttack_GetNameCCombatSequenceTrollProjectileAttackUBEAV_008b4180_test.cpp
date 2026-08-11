#include <cstdio>
#include <cstring>

// Standalone behaviour test. Reimplement CCharString locally so we can observe
// that GetName constructs from the fixed literal pointer with length -1.

static const char* g_last_str = 0;
static int         g_last_len = 999;

class CCharString {
public:
    CCharString(const char* s, int n) { g_last_str = s; g_last_len = n; }
};

class CCombatSequence_TrollProjectileAttack {
public:
    CCharString GetName() const;
};

CCharString CCombatSequence_TrollProjectileAttack::GetName() const
{
    return CCharString((const char*)0x1278a74, -1);
}

int main()
{
    CCombatSequence_TrollProjectileAttack obj;
    obj.GetName();

    bool ok = (g_last_str == (const char*)0x1278a74) && (g_last_len == -1);
    if (ok) {
        printf("GETNAME_OK ptr=0x1278a74 len=-1\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}