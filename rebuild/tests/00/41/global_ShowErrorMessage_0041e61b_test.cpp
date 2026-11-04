#include <stdio.h>
#include <string.h>

#include "fable_nui_manager.h"

// Behaviour fixture for ShowErrorMessage @ 0x0041E61B: fetches the UI
// manager singleton, then forwards the error type to its ShowErrorMessage.

static unsigned char g_manager[0xD0];
static int g_seq, g_instanceAt, g_showAt, g_showType;
static void* g_showThis;

NUISystem::CManager* FABLE_FASTCALL NUISystem::CManager::GetInstance()
{
    g_instanceAt = ++g_seq;
    return reinterpret_cast<NUISystem::CManager*>(g_manager);
}

void NUISystem::CManager::ShowErrorMessage(ERRORMESSAGE_TYPE type)
{
    g_showAt = ++g_seq; g_showThis = this; g_showType = (int)type;
}

static bool RunCase(ERRORMESSAGE_TYPE type)
{
    g_seq = g_instanceAt = g_showAt = 0; g_showType = -1; g_showThis = 0;
    memset(g_manager, 0x19, sizeof(g_manager));
    ShowErrorMessage(type);
    bool ok = g_instanceAt == 1 && g_showAt == 2 && g_showThis == g_manager && g_showType == (int)type;
    for (int i = 0; i < 0xD0; ++i)
        if (g_manager[i] != 0x19) { ok = false; break; }
    if (!ok) printf("FAIL type %d\n", (int)type);
    return ok;
}

int main()
{
    bool ok = RunCase(EMT_CONTROLLER_DISCONNECT);
    ok &= RunCase(EMT_DIRTY_DISC);
    if (ok) printf("FABLETLC_SHOWERRORMESSAGE_PASS\n");
    return ok ? 0 : 1;
}
