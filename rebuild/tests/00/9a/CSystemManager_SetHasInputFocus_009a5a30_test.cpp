#include <stdio.h>

static int g_notified = 0;
static bool g_lastFocus = false;
static void* g_lastSelf = 0;

struct FocusSink
{
    void NotifyFocus(bool focus);
};

void FocusSink::NotifyFocus(bool focus)
{
    g_notified++;
    g_lastFocus = focus;
    g_lastSelf = this;
}

struct CSystemManager
{
    char pad0[0xa];
    bool m_hasInputFocus;
    char pad1[0x58 - 0xa - 1];
    FocusSink* m_focusSink;
};

void __fastcall CSystemManager_SetHasInputFocus(CSystemManager* self, int edx, bool focus);

int main()
{
    CSystemManager m;
    for (int i = 0; i < (int)sizeof(m); i++) ((char*)&m)[i] = 0;

    // Case 1: no sink -> stores flag, no call.
    m.m_focusSink = 0;
    m.m_hasInputFocus = false;
    CSystemManager_SetHasInputFocus(&m, 0, true);
    bool ok1 = (m.m_hasInputFocus == true) && (g_notified == 0);

    // Case 2: with sink -> stores flag AND calls with same focus.
    FocusSink sink;
    m.m_focusSink = &sink;
    m.m_hasInputFocus = true;
    CSystemManager_SetHasInputFocus(&m, 0, false);
    bool ok2 = (m.m_hasInputFocus == false) && (g_notified == 1)
               && (g_lastFocus == false) && (g_lastSelf == (void*)&sink);

    if (ok1 && ok2)
        printf("SETHASINPUTFOCUS_OK\n");
    else
        printf("FAIL ok1=%d ok2=%d notified=%d\n", ok1, ok2, g_notified);
    return (ok1 && ok2) ? 0 : 1;
}