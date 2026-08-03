#include <cstdio>

enum EMusicSetType { EMS_A };

static int g_calls = 0;
static EMusicSetType g_lastType = EMS_A;
static void* g_lastThis = 0;

struct CMusicTarget
{
    void CacheMusicSet(EMusicSetType t);
};

void CMusicTarget::CacheMusicSet(EMusicSetType t)
{
    g_calls++;
    g_lastType = t;
    g_lastThis = this;
}

struct CInner
{
    char pad_0000[0x6C];
    CMusicTarget* target_006C;
};

struct CGameScriptInterface
{
    char pad_0000[0x04];
    CInner* inner_0004;
    void CacheMusicSet(EMusicSetType t) const;
};

void CGameScriptInterface::CacheMusicSet(EMusicSetType t) const
{
    inner_0004->target_006C->CacheMusicSet(t);
}

int main()
{
    CMusicTarget tgt;
    CInner inr;
    CMusicTarget* pTarget = &tgt;
    inr.target_006C = pTarget;
    CGameScriptInterface gsi;
    CInner* pInner = &inr;
    gsi.inner_0004 = pInner;

    gsi.CacheMusicSet((EMusicSetType)7);

    void* expectThis = pTarget;
    if (g_calls == 1 && g_lastType == (EMusicSetType)7 && g_lastThis == expectThis)
        printf("CACHEMUSICSET_OK\n");
    else
        printf("CACHEMUSICSET_BAD calls=%d type=%d\n", g_calls, (int)g_lastType);
    return 0;
}