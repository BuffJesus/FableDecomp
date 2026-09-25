-- Generated native draft: Gate1GuardInner. Review coverage report before use.
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
    local bVar2, bVar4, bVar5, cVar3, fVar1, fret_0, fret_00, iVar10, iVar11, iVar12, iVar9, p0, pCVar6, pcVar8, r1, r2, r3, xStack_10, xStack_20, x_stk_2c
    local alive = true
    bVar4 = false
    bVar5 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    xStack_20 = resources:NewResource()
    resources:PrepareResource(xStack_20)
    bVar2 = resources:TryAcquire(xStack_20, me, 4)
    while not bVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d03140 end
        bVar2 = resources:TryAcquire(xStack_20, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00d03140 end
    r1 = quest:GetThingWithScriptName("Gate1GuardOuter")
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    while not bVar2 do
        if not quest:GetStateBool("Gate1Open") then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            bVar2 = me:IsTalkedToByHero()
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                xStack_10 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                x_stk_2c = resources:ScriptThing(xStack_20)
                pCVar6 = x_stk_2c
                fret_0 = quest:GetHealth(pCVar6)
                fVar1 = 0.0
                if fVar1 < fret_0 then
                    iVar12 = 0
                    iVar11 = 1
                    iVar10 = 0
                    iVar9 = 0
                    pcVar8 = "TEXT_QST_009_BANDIT1B_ASIDE"
                    pCVar6 = quest:GetHero()
                    r2 = me:Speak(pCVar6, pcVar8, iVar9, (iVar10 ~= 0), (iVar11 ~= 0), (iVar12 ~= 0))
                    iVar9 = me:IsPerformingScriptTask()
                    cVar3 = iVar9
                    while cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00d02f2c end
                        iVar9 = me:IsPerformingScriptTask()
                        cVar3 = iVar9
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d02f2c end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_10)
            end
            if quest:GetStateBool("AttackedOuterGateGuards") then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    pCVar6 = quest:GetHero()
                    quest:GiveThingBestEnemyTarget(me, pCVar6)
                    resources:PrepareResource(xStack_20)
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                    until not (not bVar5)
                end
                break
            end
            bVar2 = me:MsgIsHitByHero()
            if bVar2 then
                goto LAB_00d02eb5
            else
                bVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar4 then
                    bVar4 = true
                    bVar5 = true
                    bVar2 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar2 then goto LAB_00d02eb5 end
                end
                bVar4 = true
                bVar2 = false
            end
            goto FLOW_past_lab_00d02eb5
            ::LAB_00d02eb5::
            bVar2 = true
            ::FLOW_past_lab_00d02eb5::
            if bVar5 then
                bVar5 = false
            end
            if bVar4 then
                bVar4 = false
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    xStack_10 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_2c = resources:ScriptThing(xStack_20)
                    pCVar6 = x_stk_2c
                    fret_00 = quest:GetHealth(pCVar6)
                    fVar1 = 0.0
                    if fret_00 <= fVar1 then goto LAB_00d030c7 end
                    iVar12 = 0
                    iVar11 = 1
                    iVar10 = 0
                    iVar9 = 0
                    pcVar8 = "TEXT_QST_009_BANDIT1B_ATTACKED_NEW"
                    pCVar6 = quest:GetHero()
                    r3 = me:Speak(pCVar6, pcVar8, iVar9, (iVar10 ~= 0), (iVar11 ~= 0), (iVar12 ~= 0))
                    iVar9 = me:IsPerformingScriptTask()
                    cVar3 = iVar9
                    goto LAB_00d0306b
                end
                break
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    end
    ::LAB_00d03137::
    ::LAB_00d03140::
    resources:ReleaseResource(xStack_20)
    do return end
    ::LAB_00d0306b::
    if not cVar3 then goto LAB_00d03094 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then goto LAB_00d02f2c end
    iVar9 = me:IsPerformingScriptTask()
    cVar3 = iVar9
    goto LAB_00d0306b
    ::LAB_00d02f2c::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d030ab::
    resources:DestroyMovie(xStack_10)
    goto LAB_00d03137
    ::LAB_00d03094::
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        -- LAB_00d0309f: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        goto LAB_00d030ab
    end
    ::LAB_00d030c7::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    quest:SetStateBool("AttackedOuterGateGuards", true)
    pCVar6 = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, pCVar6)
    resources:PrepareResource(xStack_20)
    repeat
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
    until not (not bVar5)
    goto LAB_00d03137
end

function Init(quest, me)
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:SetStateBool("AttackedOuterGateGuards", false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

