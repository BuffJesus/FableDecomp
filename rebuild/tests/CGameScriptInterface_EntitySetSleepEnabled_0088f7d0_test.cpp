
#include <cstdio>

static int g_ctor = 0;
static int g_dtor = 0;
static int g_call = 0;
static bool g_enabled = false;

class CCharString
{
public:
    CCharString(const char* pText, int length) { g_ctor++; (void)pText; (void)length; }
    ~CCharString() { g_dtor++; }
};

struct CScriptThing { int x; };
struct CGameScriptInterface;

typedef void(__fastcall* DispatchFn)(const CGameScriptInterface*, int, const CScriptThing*, const CCharString*, bool);

struct VTableOverlay
{
    unsigned char m_Pad00[0x938];
    DispatchFn m_Unknown938;
};

struct CGameScriptInterface
{
    const VTableOverlay* m_pVTable;
};

static void __fastcall dispatch(const CGameScriptInterface*, int, const CScriptThing*, const CCharString*, bool enabled)
{
    g_call++;
    g_enabled = enabled;
}

void __fastcall
CGameScriptInterface_EntitySetSleepEnabled(const CGameScriptInterface* self, int edx,
                                           const CScriptThing* entity, bool enabled)
{
    CCharString stateName("SG_SLEEP", -1);
    self->m_pVTable->m_Unknown938(self, edx, entity, &stateName, enabled);
}

int main()
{
    VTableOverlay vt;
    vt.m_Unknown938 = &dispatch;
    CGameScriptInterface obj;
    obj.m_pVTable = &vt;

    CScriptThing thing;
    thing.x = 7;
    CGameScriptInterface_EntitySetSleepEnabled(&obj, 0, &thing, true);

    if (g_ctor == 1 && g_dtor == 1 && g_call == 1 && g_enabled == true) {
        std::printf("CGameScriptInterface_0088f7d0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL ctor=%d dtor=%d call=%d en=%d\n", g_ctor, g_dtor, g_call, (int)g_enabled);
    return 1;
}