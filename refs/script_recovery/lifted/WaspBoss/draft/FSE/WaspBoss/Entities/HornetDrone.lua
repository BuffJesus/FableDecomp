-- Generated native draft: HornetDrone. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local bVar1, bVar2, bVar4, cVar3, iVar6, p0, p2, p3, p4, p5, p6, pCVar5, xStack_10
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        xStack_10 = resources:NewResource()
        resources:PrepareResource(xStack_10)
        bVar1 = resources:TryAcquire(xStack_10, me, 4)
        while not bVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00e12162 end
            bVar1 = resources:TryAcquire(xStack_10, me, 4)
        end
        bVar4 = false
        bVar1 = false
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            p6 = 1
            p5 = 0
            p4 = 0
            p3 = 0
            p2 = 1
            iVar6 = 0.30000001192092896
            pCVar5 = quest:GetNearestWithScriptName(me, "Q_WB_DeadBody")
            me:MoveToThing(pCVar5, iVar6, p2)
            iVar6 = me:IsPerformingScriptTask()
            cVar3 = iVar6
            while cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e12162 end
                iVar6 = me:IsPerformingScriptTask()
                cVar3 = iVar6
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                me:PlayAnimation("ST_FEED_INTO", false, false, true, false, true, false, false)
                me:PlayLoopingAnimation("ST_FEED_LOOP", -1, false, false, true)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    while true do
                        bVar2 = me:IsAwareOfHero()
                        if not (not bVar2) then break end
                        bVar2 = me:MsgIsHitByHero()
                        if bVar2 then
                            goto LAB_00e1203a
                        else
                            bVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                            if bVar4 then
                                bVar4 = true
                                bVar1 = true
                                bVar2 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                if not bVar2 then goto LAB_00e1203a end
                            end
                            bVar4 = true
                            bVar2 = false
                        end
                        goto FLOW_past_lab_00e1203a
                        ::LAB_00e1203a::
                        bVar2 = true
                        ::FLOW_past_lab_00e1203a::
                        if bVar1 then
                            bVar1 = false
                        end
                        if bVar4 then
                            bVar4 = false
                        end
                        if bVar2 then goto LAB_00e12130 end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            resources:ReleaseResource(xStack_10)
                            return
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if not bVar1 then
                        me:ClearCommands()
                        me:ClearAllActionsIncludingLoopingAnimations()
                        me:PlayAnimation("ST_FEED_OUTOF", false, false, true, true, true, false, false)
                        iVar6 = me:IsPerformingScriptTask()
                        cVar3 = iVar6
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e12162 end
                            iVar6 = me:IsPerformingScriptTask()
                            cVar3 = iVar6
                        end
                        goto LAB_00e12130
                    end
                    goto FLOW_past_lab_00e12130
                    ::LAB_00e12130::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if not bVar1 then
                        pCVar5 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCVar5)
                        resources:PrepareResource(xStack_10)
                    end
                    ::FLOW_past_lab_00e12130::
                end
            end
        end
        ::LAB_00e12162::
        resources:ReleaseResource(xStack_10)
    end
end

function Init(quest, me)
    local iVar1 = math.random(0, 32767)
    __native_entity_state:SetStateInt("EatNow", iVar1 % 200 + 1)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

