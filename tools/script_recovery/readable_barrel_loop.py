"""Structure the verified per-frame interaction body and Main loop."""
from pathlib import Path

HELPER='''    local function handleBarrelInteraction()
        if resources:IsHitByHeroExceptAbility(me, 14) then
            if quest:IsActiveThreadTerminating() then return false end
            resources:SetBarrelManHeroAllies(me)
            require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
            if not acquireBarrelControl() then return false end
            return playCarefulMovie()
        end
        local approached = false
        if __native_entity_state:GetStateInt("MyPhase") == 0 then
            approached = resources:IsHeroWithinBarrelApproachDistance(me)
        end
        if not approached then approached = me:IsTalkedToByHero() end
        if approached then
            if quest:IsActiveThreadTerminating() then return false end
            if not acquireBarrelControl() then return false end
            barrel_interaction_movie = resources:StartMovie("")
            resources:Pause(true)
            local phase = __native_entity_state:GetStateInt("MyPhase")
            local continued
            if phase ~= 0 then
                continued = playReturnInteraction(barrel_interaction_movie, phase)
            else
                continued = playInitialInteraction(barrel_interaction_movie)
            end
            barrel_interaction_movie = nil
            return continued
        end
        if __native_entity_state:GetStateInt("MyPhase") == 0
            and resources:ShouldBarrelOverhear(me, __native_entity_state:GetStateBool("OverheardYet")) then
            if quest:IsActiveThreadTerminating() then return false end
            __native_entity_state:SetStateBool("OverheardYet", true)
            resources:AddBarrelConversation(me, "TEXT_QST_048_BARRELMAN_OVERHEAR")
        end
        return true
    end
'''

LOOP='''    while not quest:IsActiveThreadTerminating() do
        if not advanceBarrelPhase() then break end
        if not handleBarrelInteraction() then break end
        quest:NewScriptFrame(me)
    end
'''


def recover(source):
    before=Path(__file__).with_name('readable_barrel_loop_before.lua').read_text()
    if source.count(before)!=1:raise ValueError('Barrel main loop source correspondence changed')
    setup=before[before.index('    barrel_resource = resources:NewResource()'):before.index('    alive = not quest:IsActiveThreadTerminating()',before.index('    warehouseGuardMarker ='))]
    replacement=HELPER+'''    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then return end
'''+setup+LOOP
    # Old post-loop movie labels have no incoming edges after verified movie
    # helpers replaced their call sites. Main-owned objects close below the loop.
    return source.replace(before,replacement,1),dict(status='structured-main-loop',semantics='Entry frame/termination, setup, top-of-loop termination, phase then interaction then frame; all exits share guard/start/resource cleanup. Removes unreachable movie cleanup labels and dead scratch declarations.')
