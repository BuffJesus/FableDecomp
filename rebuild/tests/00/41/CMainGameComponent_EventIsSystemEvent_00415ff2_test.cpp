// Behaviour fixture for CMainGameComponent::EventIsSystemEvent @ 0x00415FF2.
// Checks: only GAME_EVENT_QUIT (2) is a system event; the component and the
// event are not written.
#include "fable_game.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static bool Call(CMainGameComponent* c, const CGameEvent* e) { return static_cast<MainGameProbe*>(c)->EventIsSystemEvent(e); }
};

static unsigned char comp[0x161F0];

static bool Run(long type, bool expect)
{
    memset(comp, 0x3C, sizeof(comp));
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    CGameEvent event(type);
    CGameEvent eventBefore = event;
    bool got = MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp), &event);
    bool ok = got == expect && memcmp(before, comp, sizeof(comp)) == 0
        && memcmp(&eventBefore, &event, sizeof(event)) == 0;
    if (!ok) printf("case type=%ld failed\n", type);
    return ok;
}

int main()
{
    bool pass = Run(GAME_EVENT_QUIT, true);
    pass = Run(GAME_EVENT_UPDATE_FRAME, false) && pass;
    pass = Run(0, false) && pass;
    pass = Run(3, false) && pass;
    pass = Run(0x102, false) && pass;
    pass = Run(-1, false) && pass;
    printf(pass ? "PASS_00415ff2\n" : "FAIL_00415ff2\n");
    return pass ? 0 : 1;
}
