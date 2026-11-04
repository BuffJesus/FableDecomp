#include "fable_game_event.h"

void CGameEventPackage::AddEvent(const CGameEvent* event)
{
    if (event->Replacement)
    {
        for (unsigned long i = 0; i < NoEvents; ++i)
        {
            if (Events[i].Type == event->Type && Events[i].Player == event->Player)
            {
                Events[i] = *event;
                return;
            }
        }
    }
    Events[NoEvents] = *event;
    ++NoEvents;
}
