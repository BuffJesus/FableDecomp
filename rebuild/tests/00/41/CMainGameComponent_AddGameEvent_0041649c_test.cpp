// Behaviour fixture for CMainGameComponent::AddGameEvent @ 0x0041649C.
// PWorld is +0x24, PDisplayEngine +0x28, Client +0x13AB8. Checks: the world
// is asked whether the event is a world event; only when it is not, the
// component's EventIsSystemEvent is asked; the client sends the event when
// either says yes; then the world receives it locally and the display engine
// gets it as feedback, in that order, all with the caller's event address.
// The component is untouched.
#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_game_event.h"
#include "fable_world.h"
#include <stdio.h>
#include <string.h>

static unsigned char comp[0x161F0];
static unsigned char world[0x198];
static unsigned char display[0x100];
static const CGameEvent* expectEvent;
static int seq, worldQAt, sysQAt, sendAt, receiveAt, feedbackAt, calls;
static bool isWorld, isSystem, ok;

bool CWorld::EventIsWorldEvent(const CGameEvent* event) const
{
    ok = ok && static_cast<const void*>(this) == world && event == expectEvent;
    worldQAt = ++seq; ++calls;
    return isWorld;
}
bool CMainGameComponent::EventIsSystemEvent(const CGameEvent* event)
{
    ok = ok && static_cast<void*>(this) == comp && event == expectEvent;
    sysQAt = ++seq; ++calls;
    return isSystem;
}
void CNetworkClient::SendGameEvent(const CGameEvent* event)
{
    ok = ok && static_cast<void*>(this) == comp + 0x13AB8 && event == expectEvent;
    sendAt = ++seq; ++calls;
}
void CWorld::ReceiveLocalEvent(const CGameEvent* event)
{
    ok = ok && static_cast<void*>(this) == world && event == expectEvent;
    receiveAt = ++seq; ++calls;
}
void CDisplayEngine::ReceiveFeedbackGameEvent(const CGameEvent* event)
{
    ok = ok && static_cast<void*>(this) == display && event == expectEvent;
    feedbackAt = ++seq; ++calls;
}

static bool Run(bool worldEvent, bool systemEvent)
{
    memset(comp, 0x91, sizeof(comp));
    void* w = world; memcpy(comp + 0x24, &w, 4);
    void* d = display; memcpy(comp + 0x28, &d, 4);
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    CGameEvent event(7);
    expectEvent = &event;
    isWorld = worldEvent; isSystem = systemEvent;
    seq = worldQAt = sysQAt = sendAt = receiveAt = feedbackAt = calls = 0;
    ok = true;
    reinterpret_cast<CMainGameComponent*>(comp)->AddGameEvent(event);
    int s = 0;
    ok = ok && worldQAt == ++s;
    if (!worldEvent) ok = ok && sysQAt == ++s; else ok = ok && sysQAt == 0;
    if (worldEvent || systemEvent) ok = ok && sendAt == ++s; else ok = ok && sendAt == 0;
    ok = ok && receiveAt == ++s && feedbackAt == ++s && calls == s
        && memcmp(before, comp, sizeof(comp)) == 0;
    if (!ok) printf("case world=%d system=%d failed\n", worldEvent, systemEvent);
    return ok;
}

int main()
{
    bool pass = Run(true, false);
    pass = Run(true, true) && pass;
    pass = Run(false, true) && pass;
    pass = Run(false, false) && pass;
    printf(pass ? "PASS_0041649c\n" : "FAIL_0041649c\n");
    return pass ? 0 : 1;
}
