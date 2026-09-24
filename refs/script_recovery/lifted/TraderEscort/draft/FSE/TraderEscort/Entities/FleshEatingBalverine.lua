-- Generated native draft: FleshEatingBalverine. Review coverage report before use.
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
    local bVar1, bVar3, bVar4, b_stk_41, b_stk_43, cVar2, dist, iVar5, p0, r1, thing2, xStack_20
    local alive = true
    bVar3 = false
    bVar4 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        cVar2 = quest:GetStateBool("ShownBalverine")
        while not cVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            cVar2 = quest:GetStateBool("ShownBalverine")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            xStack_20 = resources:NewResource()
            resources:PrepareResource(xStack_20)
            bVar1 = resources:TryAcquire(xStack_20, me, 4)
            while not bVar1 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then goto LAB_00e0285e end
                bVar1 = resources:TryAcquire(xStack_20, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            b_stk_43 = not alive
            if not b_stk_43 then
                r1 = quest:GetThingWithScriptName("BalverineDinner")
                b_stk_41 = b_stk_43
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then goto LAB_00e02855 end
                    if b_stk_43 == false then
                        iVar5 = me:IsPerformingScriptTask()
                        if not iVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e02855 end
                            me:PlayAnimation("ST_MUNCH", false, false, false, true, true, false, false)
                            quest:EntityPlayObjectAnimation(r1, "EATEN_BY_BALVERINE", false)
                        end
                        dist = 25.0
                        thing2 = quest:GetHero()
                        bVar1 = quest:IsDistanceBetweenThingsUnder(me, thing2, dist)
                        if bVar1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e02855 end
                            b_stk_43 = true
                            iVar5 = me:IsPerformingScriptTask()
                            cVar2 = iVar5
                            while cVar2 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar1 = not alive
                                if bVar1 then goto LAB_00e02855 end
                                iVar5 = me:IsPerformingScriptTask()
                                cVar2 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar1 = not alive
                            if bVar1 then goto LAB_00e02855 end
                            b_stk_41 = true
                        end
                    end
                    bVar1 = me:MsgIsHitByHero()
                    if bVar1 then
                        goto LAB_00e02760
                    else
                        bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar3 then
                            bVar3 = true
                            bVar4 = true
                            bVar1 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar1 then goto LAB_00e02760 end
                        end
                        bVar3 = true
                        bVar1 = false
                    end
                    goto FLOW_past_lab_00e02760
                    ::LAB_00e02760::
                    bVar1 = true
                    ::FLOW_past_lab_00e02760::
                    if bVar4 then
                        bVar4 = false
                    end
                    if bVar3 then
                        bVar3 = false
                    end
                    if bVar1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e02855 end
                        break
                    end
                until not (b_stk_41 == false)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    me:PlayCombatAnimation("LEAP_STRAIGHT_UP", true, false)
                    iVar5 = me:IsPerformingScriptTask()
                    cVar2 = iVar5
                    while cVar2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00e02855 end
                        iVar5 = me:IsPerformingScriptTask()
                        cVar2 = iVar5
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        quest:SetStateInt("BalverinesToSurpriseHeroNeeded", quest:GetStateInt("BalverinesToSurpriseHeroNeeded") + 1)
                        quest:RemoveThing(me, false, true)
                    end
                end
                ::LAB_00e02855::
            end
            ::LAB_00e0285e::
            resources:ReleaseResource(xStack_20)
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

