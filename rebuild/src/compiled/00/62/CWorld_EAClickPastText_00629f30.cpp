// CWorld::EAClickPastText @ 00629F30; FableWin event-dispatch identity.
#include "fable_world.h"
#include "fable_player_gui.h"

void CWorld::EAClickPastText(const CGameEvent* event)
{
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetSubtitleRenderer().ClickPast();
}
