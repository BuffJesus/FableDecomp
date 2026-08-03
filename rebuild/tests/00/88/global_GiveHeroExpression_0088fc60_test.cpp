#include <stdio.h>
#include <stddef.h>

typedef unsigned char uint8_t;

class CCharString
{
public:
    CCharString(const char* s) : m_Text(s) {}
    const char* m_Text;
};

struct CThingPlayerCreature_Overlay
{
    unsigned char m_Pad00[0x91];
    uint8_t m_Flags91;
};

static int g_LearnExpressionCalls = 0;
static const char* g_LastExpr = 0;
static long g_LastValue = 0;
static bool g_LastImmediate = false;

class CThingPlayerCreature
{
public:
    bool __fastcall LearnExpression(const CCharString& expression, long value, bool immediate)
    {
        ++g_LearnExpressionCalls;
        g_LastExpr = expression.m_Text;
        g_LastValue = value;
        g_LastImmediate = immediate;
        return true;
    }
};

struct CGameScriptInterface_SelfOverlay
{
    unsigned char m_Pad00[0x14];
    void* m_Field14;
};

class CGameScriptInterface
{
public:
    void __fastcall GiveHeroExpression(const CCharString& expression, long value, bool immediate) const;
    void* m_Field14;
    unsigned char m_Pad[0x10];
};

// Test-local reimplementation mirroring the retail control flow, using
// swappable "helper" hooks in place of the fixed-address retail calls
// (0x00449970 / 0x00487DC1), so the branch/gate logic can be exercised
// without executing real retail code addresses in-process.
static CThingPlayerCreature* g_HelperHero = 0;

static void* Helper00449970(void* p)
{
    return p;
}

static CThingPlayerCreature* Helper00487DC1(void*)
{
    return g_HelperHero;
}

static void __fastcall TestGiveHeroExpression(
    CGameScriptInterface* self,
    const CCharString& expression,
    long value,
    bool immediate)
{
    CGameScriptInterface_SelfOverlay* const overlay =
        reinterpret_cast<CGameScriptInterface_SelfOverlay*>(self);

    CThingPlayerCreature* const hero =
        Helper00487DC1(Helper00449970(overlay->m_Field14));

    if ((hero != 0) &&
        ((reinterpret_cast<CThingPlayerCreature_Overlay*>(hero)->m_Flags91 & 0x01) == 0))
    {
        hero->LearnExpression(expression, value, immediate);
    }
}

int main()
{
    CGameScriptInterface iface;

    // Case 1: hero resolves to nullptr -> no call.
    g_HelperHero = 0;
    g_LearnExpressionCalls = 0;
    TestGiveHeroExpression(&iface, CCharString("SMILE"), 5, true);
    if (g_LearnExpressionCalls != 0) { printf("FAIL_NULLHERO\n"); return 1; }

    // Case 2: hero present but flag bit 0 set -> no call.
    CThingPlayerCreature_Overlay blockedHero;
    for (size_t i = 0; i < sizeof(blockedHero.m_Pad00); ++i)
    {
        blockedHero.m_Pad00[i] = 0;
    }
    blockedHero.m_Flags91 = 0x01;
    g_HelperHero = reinterpret_cast<CThingPlayerCreature*>(&blockedHero);
    g_LearnExpressionCalls = 0;
    TestGiveHeroExpression(&iface, CCharString("FROWN"), 7, false);
    if (g_LearnExpressionCalls != 0) { printf("FAIL_BLOCKEDFLAG\n"); return 1; }

    // Case 3: hero present, flag clear -> LearnExpression called with forwarded args.
    CThingPlayerCreature_Overlay okHero;
    for (size_t i = 0; i < sizeof(okHero.m_Pad00); ++i)
    {
        okHero.m_Pad00[i] = 0;
    }
    okHero.m_Flags91 = 0x00;
    g_HelperHero = reinterpret_cast<CThingPlayerCreature*>(&okHero);
    g_LearnExpressionCalls = 0;
    g_LastValue = 0;
    g_LastImmediate = false;
    TestGiveHeroExpression(&iface, CCharString("LAUGH"), 42, true);
    if (g_LearnExpressionCalls != 1) { printf("FAIL_NOTCALLED\n"); return 1; }
    if (g_LastValue != 42) { printf("FAIL_VALUE\n"); return 1; }
    if (g_LastImmediate != true) { printf("FAIL_IMMEDIATE\n"); return 1; }

    printf("GIVEHEROEXPRESSION_TEST_OK\n");
    return 0;
}