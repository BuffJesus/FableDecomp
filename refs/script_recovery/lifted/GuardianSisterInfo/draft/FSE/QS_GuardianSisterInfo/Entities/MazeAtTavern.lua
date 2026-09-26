-- Generated native draft: MazeAtTavern. Review coverage report before use.
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
    local b2, b3, bVar4, cVar5, dist, fVar3, fret_0, iVar11, iVar7, i_stk_38, p0, p0_00, p1, p4, p5, pCVar12, pCVar6, pCVar9, r1, uVar10, uVar13, xStack_1c, xStack_2c, xStack_48, xStack_54, xStack_8c, x_stk_c
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        xStack_8c = resources:NewResource()
        resources:PrepareResource(xStack_8c)
        bVar4 = resources:TryAcquire(xStack_8c, me, 3)
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00e26761 end
            bVar4 = resources:TryAcquire(xStack_8c, me, 3)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            quest:EntitySetAsOpinionSource(me, "OPINION_SOURCE_MAZE_BS_PUB")
            pCVar6 = quest:GetThingWithScriptName("M_MazeExit")
            pCVar6:GetPos()
            quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
            quest:SetThingHasInformation(me, false, true, false)
            quest:SetIsPushableByHero(me, false)
            i_stk_38 = quest:ReadGlobalGameData(0xc68)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            uVar13 = 0
            if not bVar4 then
                repeat
                    if (not quest:GetStateBool("GuardianSpokeToHero")) and (not __native_entity_state:GetStateBool("WavedOver")) then
                        dist = i_stk_38
                        pCVar6 = quest:GetHero()
                        bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, dist)
                        if bVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then break end
                            me:PlayAnimation("ST_HELLO", false, false, false, true, true, false, false)
                            iVar7 = quest:AddNewConversation(me, false, false)
                            pCVar6 = quest:GetHero()
                            quest:AddPersonToConversation(iVar7, pCVar6)
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar7, "TEXT_QST_027_MAZE_CALL_HERO_OVER_10", me, pCVar6, false)
                            __native_entity_state:SetStateBool("WavedOver", true)
                            uVar13 = xStack_8c
                        end
                    end
                    if not quest:GetStateBool("GuardianSpokeToHero") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then break end
                        bVar4 = false
                        pCVar6 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar4)
                    end
                    uVar10 = uVar13 | 1
                    xStack_8c = uVar10
                    bVar4 = me:MsgIsHitByHero()
                    if bVar4 then
                        goto LAB_00e262d9
                    else
                        uVar10 = uVar13 | 3
                        xStack_8c = uVar10
                        bVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar4 then
                            uVar10 = uVar13 | 7
                            xStack_8c = uVar10
                            bVar4 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar4 then goto LAB_00e262d9 end
                        end
                        bVar4 = false
                    end
                    goto FLOW_past_lab_00e262d9
                    ::LAB_00e262d9::
                    bVar4 = true
                    ::FLOW_past_lab_00e262d9::
                    if (uVar10 & 4) ~= 0 then
                        uVar10 = uVar10 & 0xfffffffb
                        xStack_8c = uVar10
                    end
                    if (uVar10 & 2) ~= 0 then
                        uVar10 = uVar10 & 0xfffffffd
                        xStack_8c = uVar10
                    end
                    if (uVar10 & 1) ~= 0 then
                        -- TODO(native): xStack_8c = uVar10 & 0xfffffffe;
                    end
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then break end
                        resources:PrepareResource(xStack_8c)
                        bVar4 = resources:TryAcquire(xStack_8c, me, 4)
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00e26761 end
                            bVar4 = resources:TryAcquire(xStack_8c, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then break end
                        xStack_48 = resources:StartMovie("")
                        p0_00 = 0x1
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_c = resources:ScriptThing(xStack_8c)
                        pCVar6 = x_stk_c
                        fret_0 = quest:GetHealth(pCVar6)
                        fVar3 = 0.0
                        if fVar3 < fret_0 then
                            p5 = 0
                            p4 = 1
                            iVar7 = 0
                            iVar11 = 0
                            p1 = "TEXT_QST_027_MAZE_ON_HIT_10"
                            pCVar6 = quest:GetHero()
                            r1 = me:Speak(pCVar6, p1, iVar11, (iVar7 ~= 0), (p4 ~= 0), (p5 ~= 0))
                            iVar11 = me:IsPerformingScriptTask()
                            cVar5 = iVar11
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00e26754
                                end
                                iVar11 = me:IsPerformingScriptTask()
                                cVar5 = iVar11
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00e26754
                            end
                            goto FLOW_past_lab_00e26754
                            ::LAB_00e26754::
                            resources:DestroyMovie(xStack_48)
                            break
                            ::FLOW_past_lab_00e26754::
                        end
                        __native_entity_state:SetStateInt("HitCount", __native_entity_state:GetStateInt("HitCount") + 1)
                        me:SetFriendsWithEverythingFlag(true)
                        quest:ClearThingBestEnemyTarget(me)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_48)
                    end
                    bVar4 = me:IsTalkedToByHero()
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then break end
                        resources:PrepareResource(xStack_8c)
                        bVar4 = resources:TryAcquire(xStack_8c, me, 4)
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00e26761 end
                            bVar4 = resources:TryAcquire(xStack_8c, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then break end
                        xStack_2c = resources:NewResource()
                        iVar7 = 4
                        pCVar12 = xStack_2c
                        pCVar6 = quest:GetHero()
                        resources:TryAcquire(pCVar12, pCVar6, iVar7)
                        xStack_54 = resources:NewActorMap()
                        resources:SetActor(xStack_54, "ME", xStack_8c)
                        resources:SetActor(xStack_54, "HERO", xStack_2c)
                        xStack_1c = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        resources:RunMacro("CS_GUARDIANSISTER_BOWERSTONE", xStack_54, false, true)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_1c)
                        resources:DestroyActorMap(xStack_54)
                        resources:ReleaseResource(xStack_2c)
                        quest:SetStateBool("GuardianSpokeToHero", true)
                        quest:ClearThingHasInformation(me)
                        b3 = false
                        b2 = false
                        bVar4 = false
                        pCVar9 = quest:GetActiveQuestName()
                        quest:SetQuestAsCompleted(pCVar9, bVar4, b2, b3)
                        uVar13 = 0
                        pCVar9 = quest:GetActiveQuestName()
                        quest:DeactivateQuestLater(pCVar9, uVar13)
                        quest:RemoveThing(me, false, true)
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    uVar13 = xStack_8c
                    if bVar4 then
                        resources:ReleaseResource(xStack_8c)
                        return
                    end
                until false
            end
        end
        ::LAB_00e26761::
        resources:ReleaseResource(xStack_8c)
    end
end

function Init(quest, me)
    __native_entity_state:SetStateInt("TalkCounter", 0)
    __native_entity_state:SetStateBool("WavedOver", false)
    __native_entity_state:SetStateInt("HitCount", 0)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    local pQuestName
    if quest:GetStateBool("GuardianSpokeToHero") then
        pQuestName = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pQuestName, 0)
    end
end

