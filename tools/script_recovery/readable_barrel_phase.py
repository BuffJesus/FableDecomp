"""Structure verified phase dispatch without changing state reads or cancellation."""
from pathlib import Path

HELPER='''    local function advanceBarrelPhase()
        if __native_entity_state:GetStateInt("MyPhase") == 0 then return true end
        if quest:IsActiveThreadTerminating() then return false end
        local phase = __native_entity_state:GetStateInt("MyPhase")
        if phase == 1 then
            quest:SetTimer(quest:GetStateInt("WatchTimer"), 45)
            quest:EntitySetTargetable(me, false)
            if not acquireBarrelControl() then return false end
            return walkOffFromWarehouse()
        elseif phase == 2 then
            return teleportWalkOff()
        elseif phase == 3 then
            if not acquireBarrelControl() then return false end
            quest:EntitySetCutsceneBehaviour(me, 1)
            quest:EntitySetTargetable(me, false)
            return returnToWarehouse(warehouseStartMarker)
        elseif phase == 4 then
            quest:SetStateBool("BarrelManSpokenToHeroOnReturn", true)
            resources:FaceBarrelManTowardsHero(me)
            quest:EntitySetCutsceneBehaviour(me, 2)
            quest:EntitySetTargetable(me, true)
            local stockBroken = quest:GetStateBool("BarrelBrokenPersistent")
            if resources:ShouldBarrelManThankHero(me) and not stockBroken then
                if quest:IsActiveThreadTerminating() then return false end
                if not acquireBarrelControl() then return false end
                if not playThanksMovie() then return false end
            elseif stockBroken then
                if quest:IsActiveThreadTerminating() then return false end
                if not acquireBarrelControl() then return false end
                __native_entity_state:SetStateBool("HeroLetMeDown", true)
                local brokenMovie = resources:StartMovie("")
                resources:Pause(true)
                if not playReturnInteraction(brokenMovie, 5) then return false end
            else
                if quest:IsActiveThreadTerminating() then return false end
                resources:AddBarrelConversation(me, "TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE")
                __native_entity_state:SetStateBool("HeroLetMeDown", true)
                if not showWarehouseFailure() then return false end
            end
            quest:SetCreatureBrain(me, "BRAIN_GOOD_VILLAGER_BASE")
            __native_entity_state:SetStateInt("MyPhase", 5)
        end
        return true
    end
'''


def recover(source):
    before=Path(__file__).with_name('readable_barrel_phase_before.lua').read_text()
    anchor='    local bVar3, cVar2,'
    if source.count(before)!=1 or source.count(anchor)!=1:
        raise ValueError('Barrel phase source correspondence changed')
    source=source.replace(before,'        if not advanceBarrelPhase() then goto LAB_00db6afd end\n',1)
    return source.replace(anchor,HELPER+anchor,1),dict(status='structured-phase-dispatch',semantics='Preserves initial phase test, termination query, second phase read and all per-phase action/cleanup helper order.')
