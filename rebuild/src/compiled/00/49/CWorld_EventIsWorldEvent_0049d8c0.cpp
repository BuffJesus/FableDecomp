#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_game_event_dispatch.h"

bool CWorld::EventIsWorldEvent(const CGameEvent* event) const
{
    const CGameEventDispatch* entry = Component.PeekEventDispatchTableEntry(event->GetType());
    return *reinterpret_cast<const volatile unsigned long*>(entry);
}
