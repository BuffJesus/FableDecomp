#include <stdio.h>

// Mirror of the source TU declarations. The class/struct keyword of every
// type below must match the source exactly or the mangled names will not link.
class CCharString { public: int m_Dummy; };
class CThing      { public: int m_Dummy; };
struct CScriptThing;

struct CScriptThingVTable
{
    unsigned char m_Pad0000[0x2C];
    CThing* (__fastcall* m_GetPThing)(const CScriptThing* self);
    unsigned char m_Pad0030[0x12C - 0x2C - 4];
    bool (__fastcall* m_IsAvailable)(const CScriptThing* self);
};

struct CScriptThing
{
    CScriptThingVTable* m_pVTable;
};

class CScriptConversationManager
{
public:
    void AddLineToConversation(
        long               conversationId,
        CThing*            pSpeaker,
        CThing*            pListener,
        const CCharString* pLine,
        bool               playDuringCutScenes);
};

struct CGSIAddLineToConversationWorld
{
    unsigned char               m_Pad0000[0x7C];
    CScriptConversationManager* m_pConversationManager;
};

struct CGameScriptInterface
{
    unsigned char                    m_Pad0000[0x04];
    CGSIAddLineToConversationWorld*  m_pWorld;

    void AddLineToConversation(
        long                conversationId,
        const CCharString&  line,
        bool                playDuringCutScenes,
        const CScriptThing& speaker,
        const CScriptThing& listener) const;
};

// ---------------------------------------------------------------------------
// Instrumentation
// ---------------------------------------------------------------------------
static int   g_AddCalls        = 0;
static CScriptConversationManager* g_SeenManager = 0;
static long  g_SeenId          = 0;
static CThing* g_SeenSpeaker   = 0;
static CThing* g_SeenListener  = 0;
static const CCharString* g_SeenLine = 0;
static bool  g_SeenFlag        = false;

void CScriptConversationManager::AddLineToConversation(
    long               conversationId,
    CThing*            pSpeaker,
    CThing*            pListener,
    const CCharString* pLine,
    bool               playDuringCutScenes)
{
    ++g_AddCalls;
    g_SeenManager  = this;
    g_SeenId       = conversationId;
    g_SeenSpeaker  = pSpeaker;
    g_SeenListener = pListener;
    g_SeenLine     = pLine;
    g_SeenFlag     = playDuringCutScenes;
}

static int g_SpeakerAvailCalls  = 0;
static int g_SpeakerThingCalls  = 0;
static int g_ListenerAvailCalls = 0;
static int g_ListenerThingCalls = 0;

static bool g_SpeakerAvailable  = true;
static bool g_ListenerAvailable = true;

static CThing g_SpeakerThing;
static CThing g_ListenerThing;

static const CScriptThing* g_LastSpeakerSelf  = 0;
static const CScriptThing* g_LastListenerSelf = 0;

static CScriptThing g_Speaker;
static CScriptThing g_Listener;

static bool __fastcall SpeakerIsAvailable(const CScriptThing* self)
{
    ++g_SpeakerAvailCalls;
    g_LastSpeakerSelf = self;
    return g_SpeakerAvailable;
}

static CThing* __fastcall SpeakerGetPThing(const CScriptThing* self)
{
    ++g_SpeakerThingCalls;
    g_LastSpeakerSelf = self;
    return &g_SpeakerThing;
}

static bool __fastcall ListenerIsAvailable(const CScriptThing* self)
{
    ++g_ListenerAvailCalls;
    g_LastListenerSelf = self;
    return g_ListenerAvailable;
}

static CThing* __fastcall ListenerGetPThing(const CScriptThing* self)
{
    ++g_ListenerThingCalls;
    g_LastListenerSelf = self;
    return &g_ListenerThing;
}

static void ResetCounters()
{
    g_AddCalls = 0;
    g_SeenManager = 0;
    g_SeenId = 0;
    g_SeenSpeaker = 0;
    g_SeenListener = 0;
    g_SeenLine = 0;
    g_SeenFlag = false;
    g_SpeakerAvailCalls = 0;
    g_SpeakerThingCalls = 0;
    g_ListenerAvailCalls = 0;
    g_ListenerThingCalls = 0;
    g_LastSpeakerSelf = 0;
    g_LastListenerSelf = 0;
}

static int Fail(const char* what)
{
    printf("FABLETLC_ADD_LINE_TO_CONVERSATION FAIL: %s\n", what);
    return 1;
}

int main()
{
    CScriptThingVTable speakerVT;
    CScriptThingVTable listenerVT;
    unsigned char* pSpeakerVT = (unsigned char*)&speakerVT;
    unsigned char* pListenerVT = (unsigned char*)&listenerVT;
    int i;
    for (i = 0; i < (int)sizeof(CScriptThingVTable); ++i)
    {
        pSpeakerVT[i] = 0;
        pListenerVT[i] = 0;
    }
    speakerVT.m_GetPThing    = SpeakerGetPThing;
    speakerVT.m_IsAvailable  = SpeakerIsAvailable;
    listenerVT.m_GetPThing   = ListenerGetPThing;
    listenerVT.m_IsAvailable = ListenerIsAvailable;

    g_Speaker.m_pVTable  = &speakerVT;
    g_Listener.m_pVTable = &listenerVT;

    // Layout sanity: the two vtable slots must sit at the retail byte offsets.
    void* pGetSlot = &speakerVT.m_GetPThing;
    void* pAvailSlot = &speakerVT.m_IsAvailable;
    if ((unsigned char*)pGetSlot - pSpeakerVT != 0x2C)
        return Fail("GetPThing slot offset");
    if ((unsigned char*)pAvailSlot - pSpeakerVT != 0x12C)
        return Fail("IsAvailable slot offset");

    CScriptConversationManager manager;
    CGSIAddLineToConversationWorld world;
    unsigned char* pWorld = (unsigned char*)&world;
    for (i = 0; i < (int)sizeof(CGSIAddLineToConversationWorld); ++i)
        pWorld[i] = 0;
    world.m_pConversationManager = &manager;
    void* pMgrSlot = &world.m_pConversationManager;
    if ((unsigned char*)pMgrSlot - pWorld != 0x7C)
        return Fail("conversation manager offset");

    CGameScriptInterface gsi;
    unsigned char* pGsi = (unsigned char*)&gsi;
    for (i = 0; i < (int)sizeof(CGameScriptInterface); ++i)
        pGsi[i] = 0;
    gsi.m_pWorld = &world;
    void* pWorldSlot = &gsi.m_pWorld;
    if ((unsigned char*)pWorldSlot - pGsi != 0x04)
        return Fail("world pointer offset");

    CCharString line;
    line.m_Dummy = 0x1234;
    void* pLine = &line;

    // 1) Speaker unavailable: nothing happens at all.
    ResetCounters();
    g_SpeakerAvailable = false;
    g_ListenerAvailable = true;
    gsi.AddLineToConversation(7, line, true, g_Speaker, g_Listener);
    if (g_AddCalls != 0)           return Fail("unavailable speaker still queued a line");
    if (g_SpeakerAvailCalls != 1)  return Fail("speaker availability not probed exactly once");
    if (g_SpeakerThingCalls != 0)  return Fail("speaker resolved despite being unavailable");
    if (g_ListenerAvailCalls != 0) return Fail("listener probed despite unavailable speaker");
    if (g_LastSpeakerSelf != &g_Speaker) return Fail("speaker this-pointer");

    // 2) Speaker available, listener unavailable: listener argument is NULL.
    ResetCounters();
    g_SpeakerAvailable = true;
    g_ListenerAvailable = false;
    gsi.AddLineToConversation(11, line, false, g_Speaker, g_Listener);
    if (g_AddCalls != 1)           return Fail("line not queued for available speaker");
    if (g_SeenManager != &manager) return Fail("wrong conversation manager receiver");
    if (g_SeenId != 11)            return Fail("conversation id not forwarded");
    if (g_SeenSpeaker != &g_SpeakerThing) return Fail("speaker thing not forwarded");
    if (g_SeenListener != 0)       return Fail("unavailable listener must be NULL");
    if (g_SeenLine != (const CCharString*)pLine) return Fail("line address not forwarded");
    if (g_SeenFlag != false)       return Fail("flag not forwarded (false)");
    if (g_SpeakerAvailCalls != 1)  return Fail("speaker availability probe count");
    if (g_SpeakerThingCalls != 1)  return Fail("speaker resolve count");
    if (g_ListenerAvailCalls != 1) return Fail("listener availability probe count");
    if (g_ListenerThingCalls != 0) return Fail("unavailable listener must not be resolved");

    // 3) Both available: both things forwarded, flag true.
    ResetCounters();
    g_SpeakerAvailable = true;
    g_ListenerAvailable = true;
    gsi.AddLineToConversation(-3, line, true, g_Speaker, g_Listener);
    if (g_AddCalls != 1)                    return Fail("line not queued when both available");
    if (g_SeenId != -3)                     return Fail("negative conversation id not forwarded");
    if (g_SeenSpeaker != &g_SpeakerThing)   return Fail("speaker thing (both available)");
    if (g_SeenListener != &g_ListenerThing) return Fail("listener thing (both available)");
    if (g_SeenFlag != true)                 return Fail("flag not forwarded (true)");
    if (g_SpeakerThingCalls != 1)           return Fail("speaker resolved more than once");
    if (g_ListenerThingCalls != 1)          return Fail("listener resolved more than once");
    if (g_LastListenerSelf != &g_Listener)  return Fail("listener this-pointer");

    printf("FABLETLC_ADD_LINE_TO_CONVERSATION PASS\n");
    return 0;
}