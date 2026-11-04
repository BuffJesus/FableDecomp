// Behaviour fixture for CGamePlayerInterface::IsGameActionEventInQueue without the time output (provider
// CGamePlayerInterface_IsGameActionEventInQueue_00446b10.cpp). The fixture defines the full four-argument query (retail
// 0x00445CB0): the short form must forward this, action, event type and joystick unchanged, hand it a time variable
// initialised to 0, and return the full query's answer.
#include <stdio.h>
#include "fable_player_manager.h"

static int g_fail, g_calls;
static const void* g_this;
static long g_action, g_type, g_joystick;
static float g_timeOnEntry;
static bool g_answer;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

bool CGamePlayerInterface::IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long joystick, float& time) const
{
    ++g_calls; g_this = this; g_action = action; g_type = type; g_joystick = joystick; g_timeOnEntry = time;
    time = 12.5f;
    return g_answer;
}

int main()
{
    static unsigned char raw[0x898];
    const CGamePlayerInterface* gpi = reinterpret_cast<const CGamePlayerInterface*>(raw);
    g_answer = true;
    CHECK(gpi->IsGameActionEventInQueue((EGameAction)17, (EInputButtonEventType)2, 3) == true);
    CHECK(g_calls == 1 && g_this == raw && g_action == 17 && g_type == 2 && g_joystick == 3 && g_timeOnEntry == 0.0f);
    g_answer = false;
    CHECK(gpi->IsGameActionEventInQueue((EGameAction)4, (EInputButtonEventType)0, -1) == false);
    CHECK(g_calls == 2 && g_action == 4 && g_type == 0 && g_joystick == -1 && g_timeOnEntry == 0.0f);
    if (g_fail) { printf("GAME_ACTION_IN_QUEUE_FAIL %d\n", g_fail); return 1; }
    printf("GAME_ACTION_IN_QUEUE_OK\n");
    return 0;
}
