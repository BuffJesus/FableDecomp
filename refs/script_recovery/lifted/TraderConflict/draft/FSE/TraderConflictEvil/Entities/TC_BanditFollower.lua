-- Generated native draft: TC_BanditFollower. Review coverage report before use.
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
    local bVar3, bVar4, bVar5, bVar7, bVar8, cVar6, fVar13, fVar2, fret_0, iVar11, iVar12, p0, p1, p4, p5, pCVar10, pCVar9, pQuestName, r1, r2, r3, xStack_10, xStack_20, x_stk_30
    local alive = true
    bVar7 = false
    bVar4 = false
    bVar3 = false
    bVar8 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        xStack_20 = resources:NewResource()
        pQuestName = quest:GetActiveQuestName()
        quest:EntityAttachToScript(me, pQuestName)
        quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
        quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
        pCVar9 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(me, pCVar9)
        pCVar10 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(pCVar10, me)
        quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
        cVar6 = quest:GetStateBool("QuestStartScreened")
        while not cVar6 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00df87fe end
            cVar6 = quest:GetStateBool("QuestStartScreened")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            bVar5 = true
            fVar13 = 1.0
            pCVar9 = quest:GetHero()
            quest:EntityFollowThing(me, pCVar9, fVar13, bVar5)
            r1 = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_BANDIT", 1.0)
            cVar6 = quest:GetStateBool("MissionSucceeded")
            while not cVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00df87fe end
                if not __native_entity_state:GetStateBool("SetAgainstHero") then
                    bVar8 = true
                    bVar5 = me:IsTalkedToByHero()
                    if not bVar5 then
                        bVar5 = false
                        goto FLOW_after_lab_00df82d0
                    end
                    bVar5 = true
                else
                    -- LAB_00df82d0: (native jump target)
                    bVar5 = false
                end
                ::FLOW_after_lab_00df82d0::
                if bVar8 then
                    bVar8 = false
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00df87fe end
                    bVar5 = false
                    if bVar5 ~= 0 then
                    end
                    bVar5 = resources:TryAcquire(xStack_20, me, 4)
                    while not bVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00df87fe end
                        bVar5 = resources:TryAcquire(xStack_20, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00df87fe end
                    xStack_10 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    pCVar10 = 0x1
                    quest:PauseAllNonScriptedEntities((pCVar10 ~= 0))
                    x_stk_30 = resources:ScriptThing(xStack_20)
                    pCVar9 = x_stk_30
                    fret_0 = quest:GetHealth(pCVar9)
                    fVar2 = 0.0
                    if fVar2 < fret_0 then
                        p5 = 0
                        p4 = 1
                        iVar12 = 0
                        iVar11 = 2
                        p1 = "TEXT_QST_B12_OPENING_BANDIT_ON_SPEAK_TO"
                        pCVar9 = quest:GetHero()
                        r2 = me:Speak(pCVar9, p1, iVar11, (iVar12 ~= 0), (p4 ~= 0), (p5 ~= 0))
                        iVar11 = me:IsPerformingScriptTask()
                        cVar6 = iVar11
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                resources:ReleaseResource(xStack_20)
                                return
                            end
                            iVar11 = me:IsPerformingScriptTask()
                            cVar6 = iVar11
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(xStack_10)
                            resources:DestroyMovie(xStack_20)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_10)
                end
                if not __native_entity_state:GetStateBool("SetAgainstHero") then
                    bVar7 = me:MsgIsHitByHero()
                    if not bVar7 then
                        bVar7 = true
                        bVar4 = true
                        bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar5 then
                            bVar7 = true
                            bVar4 = true
                            bVar3 = true
                            bVar5 = me:MsgIsHitByHeroSpecialAbility(me)
                            if not bVar5 then goto LAB_00df8518 end
                        end
                        bVar5 = false
                        goto FLOW_after_lab_00df851c
                    end
                    ::LAB_00df8518::
                    bVar7 = true
                    bVar5 = true
                else
                    -- LAB_00df851c: (native jump target)
                    bVar5 = false
                end
                ::FLOW_after_lab_00df851c::
                if bVar3 then
                    bVar3 = false
                end
                if bVar4 then
                    bVar4 = false
                end
                if bVar7 then
                    bVar7 = false
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00df87fe end
                    if not __native_entity_state:GetStateBool("HitWarning") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00df87fe end
                        iVar12 = quest:AddNewConversation(me, false, false)
                        pCVar9 = quest:GetHero()
                        quest:AddPersonToConversation(iVar12, pCVar9)
                        pCVar9 = quest:GetHero()
                        quest:AddLineToConversation(iVar12, "TEXT_QST_B12_BANDIT_FOLLOWER_ON_HIT_10", me, pCVar9, false)
                        __native_entity_state:SetStateBool("HitWarning", true)
                    elseif not quest:GetStateBool("HeroAttackedBandit") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00df87fe end
                        r3 = quest:GetNearestWithScriptName(me, "TC_BanditFighter")
                        bVar5 = quest:IsDistanceBetweenThingsUnder(me, r3, 15.0)
                        if bVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                resources:ReleaseResource(xStack_20)
                                return
                            end
                            quest:SetStateBool("HeroAttackedBandit", true)
                        end
                        quest:EntityStopFollowing(me)
                        pCVar9 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCVar9)
                        pCVar9 = quest:GetHero()
                        quest:EntityUnsetThingAsAllyOfThing(me, pCVar9)
                        pCVar10 = quest:GetHero()
                        quest:EntityUnsetThingAsAllyOfThing(pCVar10, me)
                        __native_entity_state:SetStateBool("SetAgainstHero", true)
                    end
                end
                if (not __native_entity_state:GetStateBool("SetAgainstHero")) and (quest:GetStateBool("HeroAttackedBandit")) then
                    fVar13 = 15.0
                    pCVar9 = quest:GetHero()
                    bVar5 = quest:IsDistanceBetweenThingsUnder(me, pCVar9, fVar13)
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00df87fe end
                        iVar12 = quest:AddNewConversation(me, false, false)
                        pCVar9 = quest:GetHero()
                        quest:AddPersonToConversation(iVar12, pCVar9)
                        pCVar9 = quest:GetHero()
                        quest:AddLineToConversation(iVar12, "TEXT_QST_B12_BANDIT_FOLLOWER_SEEKING_REVENGE_10", me, pCVar9, false)
                        quest:EntityStopFollowing(me)
                        pCVar9 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCVar9)
                        pCVar9 = quest:GetHero()
                        quest:EntityUnsetThingAsAllyOfThing(me, pCVar9)
                        pCVar10 = quest:GetHero()
                        quest:EntityUnsetThingAsAllyOfThing(pCVar10, me)
                        __native_entity_state:SetStateBool("SetAgainstHero", true)
                    end
                end
                cVar6 = quest:GetStateBool("MissionSucceeded")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar8 = not alive
            if not bVar8 then
                quest:RemoveThing(me, false, true)
            end
        end
        ::LAB_00df87fe::
        resources:ReleaseResource(xStack_20)
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("HitWarning", false)
    __native_entity_state:SetStateBool("SetAgainstHero", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
    local cVar1 = me:MsgIsKilledBy("")
    if cVar1 then
        quest:SetMasterGameState("TCEKeepBanditFollowerAlive", false)
    end
end

