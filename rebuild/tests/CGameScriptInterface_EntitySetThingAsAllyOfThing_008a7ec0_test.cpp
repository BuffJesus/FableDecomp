#include <cstdio>

class CScriptThing { public: int id; };

static const CScriptThing* g_this;
static const CScriptThing* g_a;
static const CScriptThing* g_b;
static bool g_flag;
static int g_called;

class CGameScriptInterface
{
public:
    void ImpSetEntityAsAllyOfThing(const CScriptThing& entity, const CScriptThing& otherThing, bool isAlly) const;
    void EntitySetThingAsAllyOfThing(const CScriptThing& entity, const CScriptThing& otherThing) const;
};

void CGameScriptInterface::ImpSetEntityAsAllyOfThing(const CScriptThing& entity, const CScriptThing& otherThing, bool isAlly) const
{
    g_this = (const CScriptThing*)this;
    g_a = &entity;
    g_b = &otherThing;
    g_flag = isAlly;
    g_called++;
}

void CGameScriptInterface::EntitySetThingAsAllyOfThing(const CScriptThing& entity, const CScriptThing& otherThing) const
{
    ImpSetEntityAsAllyOfThing(entity, otherThing, true);
}

int main()
{
    CGameScriptInterface gsi;
    CScriptThing a; a.id = 7;
    CScriptThing b; b.id = 9;
    gsi.EntitySetThingAsAllyOfThing(a, b);
    if (g_called == 1 && g_this == (const CScriptThing*)&gsi && g_a == &a && g_b == &b && g_flag == true) {
        std::printf("CGameScriptInterface_008a7ec0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL called=%d flag=%d\n", g_called, (int)g_flag);
    return 1;
}