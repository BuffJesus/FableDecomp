#include <cstdio>

struct CFaction;

struct FactionHolder {
    bool IsAlliedWithFaction(CFaction* faction);
};

struct CTCEnemy {
    char pad0[0x18];
    FactionHolder* m18;
    bool IsAlliedWithFaction(CFaction* faction);
};

// The forwarded method, defined locally for the standalone test.
static int g_lastArg = 0;
bool FactionHolder::IsAlliedWithFaction(CFaction* faction)
{
    g_lastArg = (int)faction;
    return faction == (CFaction*)0x1234;
}

bool CTCEnemy::IsAlliedWithFaction(CFaction* faction)
{
    if (faction == 0)
        return false;
    FactionHolder* f = this->m18;
    if (f == 0)
        return false;
    return f->IsAlliedWithFaction(faction);
}

int main()
{
    CTCEnemy e;
    FactionHolder h;
    e.m18 = &h;

    // null faction -> false, holder not consulted (g_lastArg untouched)
    g_lastArg = -1;
    bool r1 = e.IsAlliedWithFaction((CFaction*)0);
    bool consulted1 = (g_lastArg != -1);

    // null holder -> false, holder not consulted
    CTCEnemy e2;
    e2.m18 = 0;
    g_lastArg = -1;
    bool r2 = e2.IsAlliedWithFaction((CFaction*)0x1234);
    bool consulted2 = (g_lastArg != -1);

    // forwarded true
    bool r3 = e.IsAlliedWithFaction((CFaction*)0x1234);
    bool consulted3 = (g_lastArg == 0x1234);

    // forwarded false
    bool r4 = e.IsAlliedWithFaction((CFaction*)0x9999);

    if (!r1 && !consulted1 && !r2 && !consulted2 && r3 && consulted3 && !r4)
        printf("PASS_ISALLIED_OK\n");
    else
        printf("FAIL r1=%d c1=%d r2=%d c2=%d r3=%d r4=%d\n",
               r1, consulted1, r2, consulted2, r3, r4);
    return (!r1 && !consulted1 && !r2 && !consulted2 && r3 && !r4) ? 0 : 1;
}