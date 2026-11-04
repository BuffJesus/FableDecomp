#include "fable_world.h"
#include "fable_game.h"
#include "fable_game_event.h"
void CWorld::LEASetExclusiveMode(const CGameEvent* event)
{
    CGameEventDataPackage event_data;
    event->GetDataPackage(&event_data);
    unsigned char data_on;
    event_data.GetData(1, &data_on);
    bool on = data_on != 0;
    Component.SetExclusiveMode(on);
}
