#include <cstdio>

struct CListNode
{
    CListNode* m_pNext;
    CListNode* m_pPrevious;
};

struct CMessageList
{
    CListNode* m_pHead;
};

struct CWorldOverlay
{
    unsigned char m_Pad00[0x60];
    void*         m_pMessageManager;
};

struct CGameScriptInterface
{
    void*          m_pVTable;
    CWorldOverlay* m_pWorld;
};

static const CGameScriptInterface* g_pBeginSelf = 0;
static const CGameScriptInterface* g_pEndSelf = 0;
static void* g_pObservedManager = 0;
static void* g_pObservedCallbackRegister = 0;
static void* g_pObservedFilter = 0;
static void* g_pObservedCallback = 0;
static unsigned long g_ObservedEventType = 0;
static unsigned long g_ObservedRangeBegin = 0;
static unsigned long g_ObservedRangeEnd = 0;
static void* g_pObservedCallbackWorld = 0;
static void* g_pObservedCallbackMessages = 0;
static unsigned long g_BeginValue = 0;
static unsigned long g_EndValue = 0;
static unsigned long g_BeginCalls = 0;
static unsigned long g_EndCalls = 0;
static unsigned long g_DispatchCalls = 0;
static bool g_AppendMessage = false;
static CListNode g_AppendedNode = {};

extern "C" unsigned long __fastcall
Sub_GetLevelLoadedRangeBegin(const CGameScriptInterface* self)
{
    g_pBeginSelf = self;
    ++g_BeginCalls;
    return g_BeginValue;
}

extern "C" unsigned long __fastcall
Sub_GetLevelLoadedRangeEnd(const CGameScriptInterface* self)
{
    g_pEndSelf = self;
    ++g_EndCalls;
    return g_EndValue;
}

extern "C" long __fastcall Sub_ForEachLevelLoadedMessage(
    void* pManager,
    void* pCallbackRegister,
    void* pFilter,
    void* pCallback)
{
    g_pObservedManager = pManager;
    g_pObservedCallbackRegister = pCallbackRegister;
    g_pObservedFilter = pFilter;
    g_pObservedCallback = pCallback;
    ++g_DispatchCalls;

    unsigned long** filterWords = (unsigned long**)pFilter;
    unsigned long* eventType = filterWords[0];
    unsigned long* range = filterWords[1];
    g_ObservedEventType = *eventType;
    g_ObservedRangeBegin = range[0];
    g_ObservedRangeEnd = range[1];

    void** callbackWords = (void**)pCallback;
    g_pObservedCallbackWorld = callbackWords[0];
    g_pObservedCallbackMessages = callbackWords[1];

    if (g_AppendMessage)
    {
        CMessageList* messages = (CMessageList*)callbackWords[1];
        CListNode* head = messages->m_pHead;
        g_AppendedNode.m_pNext = head;
        g_AppendedNode.m_pPrevious = head;
        head->m_pNext = &g_AppendedNode;
        head->m_pPrevious = &g_AppendedNode;
    }
    return 0;
}

bool __fastcall CGameScriptInterface_MsgOnLevelLoaded(
    const CGameScriptInterface* self,
    int edxUnused,
    void* pMessages);

static void ResetObservations()
{
    g_pBeginSelf = 0;
    g_pEndSelf = 0;
    g_pObservedManager = 0;
    g_pObservedCallbackRegister = 0;
    g_pObservedFilter = 0;
    g_pObservedCallback = 0;
    g_ObservedEventType = 0;
    g_ObservedRangeBegin = 0;
    g_ObservedRangeEnd = 0;
    g_pObservedCallbackWorld = 0;
    g_pObservedCallbackMessages = 0;
    g_BeginCalls = 0;
    g_EndCalls = 0;
    g_DispatchCalls = 0;
    g_AppendMessage = false;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_0089ac10_TEST FAIL: %s\n", message);
    return 1;
}

static int VerifyDispatchLayout(
    const CGameScriptInterface* self,
    CMessageList* messages,
    void* manager)
{
    if (g_BeginCalls != 1 ||
        g_EndCalls != 1 ||
        g_DispatchCalls != 1 ||
        g_pBeginSelf != self ||
        g_pEndSelf != self)
        return Fail("range helper call order/receiver");

    if (g_pObservedManager != manager ||
        g_pObservedCallbackRegister != g_pObservedCallback)
        return Fail("manager/callback register");

    if (g_ObservedEventType != 0x24 ||
        g_ObservedRangeBegin != g_BeginValue ||
        g_ObservedRangeEnd != g_EndValue)
        return Fail("event/range filter layout");

    if (g_pObservedCallbackWorld != self->m_pWorld ||
        g_pObservedCallbackMessages != messages)
        return Fail("callback layout");

    return 0;
}

int main()
{
    CWorldOverlay world = {};
    CGameScriptInterface gsi = {};
    CListNode head = {};
    CMessageList messages = {};
    void* manager = (void*)0x12345678;

    world.m_pMessageManager = manager;
    gsi.m_pWorld = &world;
    head.m_pNext = &head;
    head.m_pPrevious = &head;
    messages.m_pHead = &head;
    g_BeginValue = 0x10203040;
    g_EndValue = 0x50607080;

    ResetObservations();
    if (CGameScriptInterface_MsgOnLevelLoaded(&gsi, 0, &messages))
        return Fail("empty list result");
    int layoutResult = VerifyDispatchLayout(&gsi, &messages, manager);
    if (layoutResult != 0)
        return layoutResult;

    head.m_pNext = &head;
    head.m_pPrevious = &head;
    ResetObservations();
    g_AppendMessage = true;
    if (!CGameScriptInterface_MsgOnLevelLoaded(&gsi, 0, &messages))
        return Fail("post-dispatch non-empty result");
    layoutResult = VerifyDispatchLayout(&gsi, &messages, manager);
    if (layoutResult != 0)
        return layoutResult;

    std::printf("CGameScriptInterface_0089ac10_TEST PASS\n");
    return 0;
}
