#define FABLE_THING_INLINE_CONST_GETTC
#include "fable_display_engine.h"
#include "fable_game.h"
#include "fable_player_manager.h"
#include "fable_world.h"
void CDisplayEngine::RenderHeroFuckShit(const CInterpolationInfo& info)
{
    const CThingPlayerCreature* hero = static_cast<const CPlayerManager*>(PlayerManager)->GetMainPlayer().PeekPControlledCreature();
    if (IS_THING_ALIVE(hero) && hero->HasTC(TCI_REGION_DISPLAY))
        static_cast<const CTCRegionDisplay&>(hero->GetTC(TCI_REGION_DISPLAY)).GlobalDraw(info);
}
