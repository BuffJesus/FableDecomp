#include <stdio.h>
#include <string.h>

#include "fable_game.h"
#include "fable_nui_manager.h"

// Behaviour fixture for CNewFrontendGameComponent::Update @ 0x0042DC94.
// The component and the NUI manager are raw retail-sized buffers so the
// fixture checks retail offsets: m_pFrontEndManager +0xB4,
// m_fLastRealUpdateTime +0xC0, CManager::m_bHasErrorMessage +0x9C.

namespace
{
    int g_seq;
    double g_times[2];
    int g_timeCalls;
    int g_timeAt[2];

    bool g_disconnected;
    int g_discAt;
    const void* g_discThis;

    bool g_canStart;
    int g_canStartAt;
    const void* g_canStartThis;

    unsigned char g_managerBytes[0xD0];
    int g_instanceCalls;

    int g_showAt;
    int g_showArg;
    int g_clearAt;
    const void* g_clearThis;

    int g_updateAt;
    const void* g_updateThis;
    float g_updateArg;

    unsigned char g_frontEndBytes[0x100];

    void Reset()
    {
        g_seq = 0;
        g_timeCalls = 0;
        g_timeAt[0] = g_timeAt[1] = 0;
        g_discAt = 0; g_discThis = 0;
        g_canStartAt = 0; g_canStartThis = 0;
        g_instanceCalls = 0;
        g_showAt = 0; g_showArg = -1;
        g_clearAt = 0; g_clearThis = 0;
        g_updateAt = 0; g_updateThis = 0; g_updateArg = -1.0f;
        memset(g_managerBytes, 0, sizeof(g_managerBytes));
    }
}

double FABLE_FASTCALL GFGetTime()
{
    int call = g_timeCalls++;
    if (call < 2) g_timeAt[call] = ++g_seq;
    return call < 2 ? g_times[call] : 0.0;
}

bool CNewFrontendGameComponent::IsControllerDisconnected()
{
    g_discAt = ++g_seq;
    g_discThis = this;
    return g_disconnected;
}

NUISystem::CManager* FABLE_FASTCALL NUISystem::CManager::GetInstance()
{
    ++g_instanceCalls;
    return reinterpret_cast<NUISystem::CManager*>(g_managerBytes);
}

void NUISystem::CManager::ClearErrorMessage()
{
    g_clearAt = ++g_seq;
    g_clearThis = this;
}

bool NUISystem::CFrontEndManager::CanStartAttractMode()
{
    g_canStartAt = ++g_seq;
    g_canStartThis = this;
    return g_canStart;
}

void NUISystem::CFrontEndManager::Update(float timePassed)
{
    g_updateAt = ++g_seq;
    g_updateThis = this;
    g_updateArg = timePassed;
}

void FABLE_FASTCALL ShowErrorMessage(ERRORMESSAGE_TYPE type)
{
    g_showAt = ++g_seq;
    g_showArg = (int)type;
}

static unsigned char g_component[0x148];

static bool RunCase(
    const char* name, bool disconnected, bool canStart, bool hasError,
    double t0, double last, double t1, float expectDt,
    bool expectCanStart, bool expectShow, bool expectClear)
{
    Reset();
    memset(g_component, 0xA5, sizeof(g_component));
    void* fe = g_frontEndBytes;
    memcpy(g_component + 0xB4, &fe, 4);
    memcpy(g_component + 0xC0, &last, 8);
    g_disconnected = disconnected;
    g_canStart = canStart;
    g_managerBytes[0x9C] = hasError ? 1 : 0;
    g_times[0] = t0;
    g_times[1] = t1;

    CNewFrontendGameComponent* self =
        reinterpret_cast<CNewFrontendGameComponent*>(g_component);
    self->Update();

    bool ok = true;
    // GFGetTime first, then the disconnect probe, then the tick, then the stamp.
    ok = ok && g_timeCalls == 2 && g_timeAt[0] == 1;
    ok = ok && g_discAt == 2 && g_discThis == g_component;
    if (expectCanStart)
        ok = ok && g_canStartAt == 3 && g_canStartThis == g_frontEndBytes;
    else
        ok = ok && g_canStartAt == 0;
    if (expectShow)
        ok = ok && g_showAt != 0 && g_showArg == 0 && g_showAt < g_updateAt;
    else
        ok = ok && g_showAt == 0;
    if (expectClear)
        ok = ok && g_clearAt != 0 && g_clearThis == g_managerBytes && g_clearAt < g_updateAt;
    else
        ok = ok && g_clearAt == 0;
    ok = ok && g_updateThis == g_frontEndBytes && g_updateArg == expectDt;
    ok = ok && g_timeAt[1] == g_updateAt + 1;
    double stored;
    memcpy(&stored, g_component + 0xC0, 8);
    ok = ok && stored == t1;
    // Neighbours untouched.
    for (int i = 0; i < 0x148; ++i)
    {
        if (i >= 0xB4 && i < 0xB8) continue;
        if (i >= 0xC0 && i < 0xC8) continue;
        if (g_component[i] != 0xA5) { ok = false; break; }
    }
    if (!ok) printf("FAIL %s\n", name);
    return ok;
}

int main()
{
    bool ok = true;
    float small = (float)(10.05 - 10.0);
    ok &= RunCase("disc-noattract-noerror", true, false, false, 10.05, 10.0, 11.0, small, true, true, false);
    ok &= RunCase("disc-noattract-error", true, false, true, 10.05, 10.0, 11.0, small, true, false, false);
    ok &= RunCase("disc-attract-error", true, true, true, 10.05, 10.0, 11.0, small, true, false, true);
    ok &= RunCase("disc-attract-noerror", true, true, false, 10.05, 10.0, 11.0, small, true, false, false);
    ok &= RunCase("conn-noerror-clamp", false, false, false, 10.5, 10.0, 12.0, 0.1f, false, false, false);
    ok &= RunCase("conn-error", false, false, true, 10.05, 10.0, 12.0, small, false, false, true);
    ok &= RunCase("exact-0.1", false, false, false, 0.1f, 0.0, 1.0, 0.1f, false, false, false);
    if (ok) printf("FABLETLC_NEWFRONTEND_UPDATE_PASS\n");
    return ok ? 0 : 1;
}