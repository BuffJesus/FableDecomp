#include <cstdio>
#include <cstring>

class CScriptThing;

struct CCharString
{
    void* m_pRep; // 0x00
    CCharString(const char* pText, int length);
    ~CCharString();
};

struct CGameScriptInterface
{
    void** vt; // +0x0
    void EntitySetOpinionReactionsEnabled(
        const CScriptThing& entity, int enabled) const;
};

typedef void(__fastcall* SlotFn)(
    const CGameScriptInterface* pThis,
    int edxDummy,
    const void* entity,
    const CCharString* tcName,
    int enabled);

const char kName[] = "SG_OPINION_REACTION_ROOT";

static const char* g_ctorText = 0;
static int g_ctorLen = 999;
static int g_slotCalls = 0;
static bool g_slotEnabled = false;
static const void* g_slotThis = 0;
static const void* g_slotEntity = 0;

CCharString::CCharString(const char* pText, int length)
{
    g_ctorText = pText;
    g_ctorLen = length;
    m_pRep = 0;
}
CCharString::~CCharString() {}

static void __fastcall FakeSlot(
    const CGameScriptInterface* pThis,
    int /*edxDummy*/,
    const void* entity,
    const CCharString* /*tcName*/,
    int enabled)
{
    ++g_slotCalls;
    g_slotEnabled = enabled != 0;
    g_slotThis = pThis;
    g_slotEntity = entity;
}

void CGameScriptInterface::EntitySetOpinionReactionsEnabled(
    const CScriptThing& entity, int enabled) const
{
    CCharString tcName(kName, -1);
    SlotFn slot = (SlotFn)(this->vt[0x938 / 4]);
    slot(this, 0, (const void*)&entity, &tcName, enabled);
}

int main()
{
    static void* vtbl[0x938 / 4 + 1];
    vtbl[0x938 / 4] = (void*)&FakeSlot;
    CGameScriptInterface obj;
    obj.vt = vtbl;

    const CScriptThing* entity = (const CScriptThing*)0x1234;
    obj.EntitySetOpinionReactionsEnabled(*entity, 1);

    bool ok = true;
    if (g_slotCalls != 1) ok = false;
    if (!g_slotEnabled) ok = false;
    if (g_slotThis != &obj) ok = false;
    if (g_slotEntity != (const void*)0x1234) ok = false;
    if (g_ctorLen != -1) ok = false;
    if (g_ctorText == 0 || std::strcmp(g_ctorText, "SG_OPINION_REACTION_ROOT") != 0) ok = false;

    if (ok)
    {
        std::printf("CGameScriptInterface_0088f810_TEST PASS\n");
        return 0;
    }
    std::printf("CGameScriptInterface_0088f810_TEST FAIL calls=%d en=%d len=%d\n",
                g_slotCalls, (int)g_slotEnabled, g_ctorLen);
    return 1;
}