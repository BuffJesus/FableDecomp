-- Generated native draft: InfectedBalverine. Review coverage report before use.
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
    local bVar1, bVar2, bVar3, b_stk_36, cVar4, dist, iVar5, p0, thing2, v_stk_24, xStack_20
    local alive = true
    v_stk_24 = 0
    alive = quest:NewScriptFrame(me)
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
            if bVar1 then goto LAB_00e02c78 end
            bVar1 = resources:TryAcquire(xStack_20, me, 4)
        end
        bVar3 = false
        bVar1 = false
        alive = not quest:IsActiveThreadTerminating()
        b_stk_36 = not alive
        if not b_stk_36 then
            v_stk_24 = quest:ReadGlobalGameData(0xe0c)
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e02c78 end
                iVar5 = me:IsPerformingScriptTask()
                if not iVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e02c78 end
                    me:PlayLoopingAnimation("ST_MUNCH", -1, false, false, false)
                end
                dist = v_stk_24
                thing2 = quest:GetHero()
                bVar2 = quest:IsDistanceBetweenThingsUnder(me, thing2, dist)
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e02c78 end
                    me:ClearAllActionsIncludingLoopingAnimations()
                    b_stk_36 = true
                end
                bVar2 = me:MsgIsHitByHero()
                if bVar2 then
                    goto LAB_00e02b66
                else
                    bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar3 then
                        bVar3 = true
                        bVar1 = true
                        bVar2 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar2 then goto LAB_00e02b66 end
                    end
                    bVar3 = true
                    bVar2 = false
                end
                goto FLOW_past_lab_00e02b66
                ::LAB_00e02b66::
                bVar2 = true
                ::FLOW_past_lab_00e02b66::
                if bVar1 then
                    bVar1 = false
                end
                if bVar3 then
                    bVar3 = false
                end
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e02c78 end
                    b_stk_36 = true
                end
                if quest:GetStateBool("MissionFailed") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then goto LAB_00e02c78 end
                    break
                end
            until not (b_stk_36 == false)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:SetStateBool("InfectedTraderCanGetUp", true)
                me:PlayCombatAnimation("LEAP_STRAIGHT_UP", true, false)
                iVar5 = me:IsPerformingScriptTask()
                cVar4 = iVar5
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then goto LAB_00e02c78 end
                    iVar5 = me:IsPerformingScriptTask()
                    cVar4 = iVar5
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if not bVar1 then
                    quest:RemoveThing(me, false, true)
                end
            end
        end
        ::LAB_00e02c78::
        resources:ReleaseResource(xStack_20)
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

