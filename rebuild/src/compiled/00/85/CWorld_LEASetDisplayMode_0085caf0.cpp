#include "fable_world.h"
#include "fable_game.h"
#include "fable_game_event.h"
void CWorld::LEASetDisplayMode(const CGameEvent* event)
{
    CGameEventDataPackage event_data;
    event->GetDataPackage(&event_data);
    unsigned short width;
    unsigned short height;
    unsigned char depth;
    event_data.GetData(2, &width);
    event_data.GetData(2, &height);
    event_data.GetData(1, &depth);
    Component.SetDisplayMode(width, height, depth);
}
