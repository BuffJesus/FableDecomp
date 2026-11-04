#include "fable_display_engine.h"
#include "fable_game.h"
#include "fable_player_manager.h"
#include "fable_world.h"
void CDisplayEngine::RenderHeroDrunkenness(const CInterpolationInfo& info)
{
    CThingPlayerCreature* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCHeroStats* stats;
    if (hero != 0 && hero->GetTC(&stats) && stats != 0)
        stats->RenderHeroDrunkenness(Engine3D);
}
