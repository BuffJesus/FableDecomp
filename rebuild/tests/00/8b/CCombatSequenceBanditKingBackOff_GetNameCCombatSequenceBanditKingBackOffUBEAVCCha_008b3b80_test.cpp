#include <cstdio>
#include <cstring>

struct CCharString {
    int d;
    CCharString(const char* s, int n);
};

char kNameStr[] = "CombatSequence_BanditKingBackOff";

struct CCombatSequence_BanditKingBackOff {
    int x;
    CCharString GetName();
};

static const char* g_last_s;
static int g_last_n;

CCharString::CCharString(const char* s, int n)
{
    d = 0x1234;
    g_last_s = s;
    g_last_n = n;
}

int main()
{
    CCombatSequence_BanditKingBackOff obj; obj.x = 7;
    CCharString r = obj.GetName();
    if (r.d == 0x1234 && g_last_n == -1 && strcmp(g_last_s, "CombatSequence_BanditKingBackOff") == 0) {
        printf("008b3b80_TEST PASS\n");
    } else {
        printf("FAIL d=%d n=%d\n", r.d, g_last_n);
    }
    return 0;
}