// Behaviour fixture for CMainGameComponent::ProcessEvent @ 0x00415FE0.
// The component is a retail-sized buffer whose vptr is a fake table; slot 4
// (+0x10) is SetQuit. Checks: GAME_EVENT_QUIT calls SetQuit once on the
// component; every other type calls nothing; the component and the event are
// left untouched.
#include "fable_game.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c, const CGameEvent* e) { static_cast<MainGameProbe*>(c)->ProcessEvent(e); }
};

static unsigned char comp[0x161F0];
static void* vtbl[10];
static int quitCalls, otherCalls;
static const void* quitThis;

static void __fastcall FakeSetQuit(void* self, void*) { ++quitCalls; quitThis = self; }
static void __fastcall FakeOther(void*, void*) { ++otherCalls; }

static bool Run(long type, int expectQuit)
{
    for (int i = 0; i < 10; ++i) vtbl[i] = (void*)FakeOther;
    vtbl[4] = (void*)FakeSetQuit;
    memset(comp, 0x5A, sizeof(comp));
    void* vt = vtbl;
    memcpy(comp, &vt, 4);
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    CGameEvent event(type);
    CGameEvent eventBefore = event;
    quitCalls = otherCalls = 0;
    quitThis = 0;
    MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp), &event);
    bool ok = quitCalls == expectQuit && otherCalls == 0
        && (expectQuit == 0 || quitThis == comp)
        && memcmp(before, comp, sizeof(comp)) == 0
        && memcmp(&eventBefore, &event, sizeof(event)) == 0;
    if (!ok) printf("case type=%ld failed\n", type);
    return ok;
}

int main()
{
    bool pass = Run(GAME_EVENT_QUIT, 1);
    pass = Run(GAME_EVENT_UPDATE_FRAME, 0) && pass;
    pass = Run(0, 0) && pass;
    pass = Run(3, 0) && pass;
    pass = Run(-2, 0) && pass;
    printf(pass ? "PASS_00415fe0\n" : "FAIL_00415fe0\n");
    return pass ? 0 : 1;
}
