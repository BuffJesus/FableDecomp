-- Generated native draft: Assassin3. Review coverage report before use.
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
    local bVar3, bVar4, bVar5, bVar6, cVar7, fVar2, fret_0, fret_00, iVar10, iVar11, iVar12, iVar13, p0, pCVar8, pcVar9, r1, r2, this_00, xStack_10, xStack_20, xStack_30, x_stk_3c, x_stk_48
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    xStack_30 = resources:NewResource()
    resources:PrepareResource(xStack_30)
    bVar3 = resources:TryAcquire(xStack_30, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d06249 end
        bVar3 = resources:TryAcquire(xStack_30, me, 4)
    end
    bVar6 = false
    bVar3 = false
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        cVar7 = quest:GetStateBool("AssassinsUnderAttack")
        bVar4 = false
        while (not cVar7 and (not quest:GetStateBool("AssassinCutsceneTriggered"))) do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00d06249 end
            bVar5 = me:MsgIsHitByHero()
            if bVar5 then
                goto LAB_00d06301
            else
                bVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar6 then
                    bVar6 = true
                    bVar3 = true
                    bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar5 then goto LAB_00d06301 end
                end
                bVar6 = true
                bVar5 = false
            end
            goto FLOW_past_lab_00d06301
            ::LAB_00d06301::
            bVar5 = true
            ::FLOW_past_lab_00d06301::
            if bVar3 then
                bVar3 = false
            end
            if bVar6 then
                bVar6 = false
            end
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00d06249 end
                quest:SetStateBool("AssassinsUnderAttack", true)
            end
            bVar5 = me:IsTalkedToByHero()
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00d06249 end
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00d06249 end
                    xStack_10 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_3c = resources:ScriptThing(xStack_30)
                    pCVar8 = x_stk_3c
                    fret_00 = quest:GetHealth(pCVar8)
                    fVar2 = 0.0
                    if fVar2 < fret_00 then
                        iVar13 = 0
                        iVar12 = 1
                        iVar11 = 0
                        iVar10 = 0
                        pcVar9 = "TEXT_QST_009_ASSASSIN3_REPEAT"
                        pCVar8 = quest:GetHero()
                        r1 = me:Speak(pCVar8, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                        iVar10 = me:IsPerformingScriptTask()
                        cVar7 = iVar10
                        while cVar7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                resources:ReleaseResource(xStack_30)
                                return
                            end
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            -- LAB_00d0667d: (native jump target)
                            resources:DestroyMovie(xStack_10)
                            resources:ReleaseResource(xStack_30)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = xStack_10
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d06249 end
                    xStack_20 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_48 = resources:ScriptThing(xStack_30)
                    pCVar8 = x_stk_48
                    fret_0 = quest:GetHealth(pCVar8)
                    fVar2 = 0.0
                    if fVar2 < fret_0 then
                        iVar13 = 0
                        iVar12 = 1
                        iVar11 = 0
                        iVar10 = 0
                        pcVar9 = "TEXT_QST_009_ASSASSIN3_INTRO"
                        pCVar8 = quest:GetHero()
                        r2 = me:Speak(pCVar8, pcVar9, iVar10, (iVar11 ~= 0), (iVar12 ~= 0), (iVar13 ~= 0))
                        iVar10 = me:IsPerformingScriptTask()
                        cVar7 = iVar10
                        while cVar7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_20)
                                resources:ReleaseResource(xStack_30)
                                return
                            end
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            -- LAB_00d0662a: (native jump target)
                            resources:DestroyMovie(xStack_20)
                            resources:ReleaseResource(xStack_30)
                            return
                        end
                    end
                    bVar4 = true
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = xStack_20
                end
            end
            cVar7 = quest:GetStateBool("AssassinsUnderAttack")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            if quest:GetStateBool("AssassinCutsceneTriggered") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d06249 end
                resources:PrepareResource(xStack_30)
            end
            quest:ClearThingHasInformation(me)
        end
    end
    ::LAB_00d06249::
    resources:ReleaseResource(xStack_30)
end

function Init(quest, me)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:SetIsPushableByHero(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

