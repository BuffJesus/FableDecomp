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
    local bVar2, bVar3, bVar4, bVar6, bVar7, cVar5, fVar1, fVar12, fret_0, iVar10, iVar11, p0, p1, p4, p5, pCVar8, pCVar9, pQuestName, r1, r2, r3, xStack_10, xStack_20, x_stk_2c
    local alive = true
    bVar6 = false
    bVar3 = false
    bVar2 = false
    bVar7 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        xStack_20 = resources:NewResource()
        pQuestName = quest:GetActiveQuestName()
        quest:EntityAttachToScript(me, pQuestName)
        quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
        quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
        pCVar8 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(me, pCVar8)
        pCVar9 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(pCVar9, me)
        quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
        cVar5 = quest:GetStateBool("QuestStartScreened")
        while not cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00df87fe end
            cVar5 = quest:GetStateBool("QuestStartScreened")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            bVar4 = true
            fVar12 = 1.0
            pCVar8 = quest:GetHero()
            quest:EntityFollowThing(me, pCVar8, fVar12, bVar4)
            r1 = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_BANDIT", 1.0)
            cVar5 = quest:GetStateBool("MissionSucceeded")
            while not cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00df87fe end
                if not __native_entity_state:GetStateBool("SetAgainstHero") then
                    bVar7 = true
                    bVar4 = me:IsTalkedToByHero()
                    if not bVar4 then goto LAB_00df82d0 end
                    bVar4 = true
                else
                    goto LAB_00df82d0
                end
                goto FLOW_past_lab_00df82d0
                ::LAB_00df82d0::
                bVar4 = false
                ::FLOW_past_lab_00df82d0::
                if bVar7 then
                    bVar7 = false
                end
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00df87fe end
                    resources:PrepareResource(xStack_20)
                    bVar4 = resources:TryAcquire(xStack_20, me, 4)
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00df87fe end
                        bVar4 = resources:TryAcquire(xStack_20, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00df87fe end
                    xStack_10 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_2c = resources:ScriptThing(xStack_20)
                    pCVar8 = x_stk_2c
                    fret_0 = quest:GetHealth(pCVar8)
                    fVar1 = 0.0
                    if fVar1 < fret_0 then
                        p5 = 0
                        p4 = 1
                        iVar11 = 0
                        iVar10 = 2
                        p1 = "TEXT_QST_B12_OPENING_BANDIT_ON_SPEAK_TO"
                        pCVar8 = quest:GetHero()
                        r2 = me:Speak(pCVar8, p1, iVar10, (iVar11 ~= 0), (p4 ~= 0), (p5 ~= 0))
                        iVar10 = me:IsPerformingScriptTask()
                        cVar5 = iVar10
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_10)
                                resources:ReleaseResource(xStack_20)
                                return
                            end
                            iVar10 = me:IsPerformingScriptTask()
                            cVar5 = iVar10
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_10)
                            resources:ReleaseResource(xStack_20)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_10)
                end
                if not __native_entity_state:GetStateBool("SetAgainstHero") then
                    bVar6 = me:MsgIsHitByHero()
                    if not bVar6 then
                        bVar6 = true
                        bVar3 = true
                        bVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar4 then
                            bVar6 = true
                            bVar3 = true
                            bVar2 = true
                            bVar4 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar4 then goto LAB_00df8518 end
                        end
                        goto LAB_00df851c
                    end
                    ::LAB_00df8518::
                    bVar6 = true
                    bVar4 = true
                else
                    goto LAB_00df851c
                end
                goto FLOW_past_lab_00df851c
                ::LAB_00df851c::
                bVar4 = false
                ::FLOW_past_lab_00df851c::
                if bVar2 then
                    bVar2 = false
                end
                if bVar3 then
                    bVar3 = false
                end
                if bVar6 then
                    bVar6 = false
                end
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00df87fe end
                    if not __native_entity_state:GetStateBool("HitWarning") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00df87fe end
                        iVar11 = quest:AddNewConversation(me, false, false)
                        pCVar8 = quest:GetHero()
                        quest:AddPersonToConversation(iVar11, pCVar8)
                        pCVar8 = quest:GetHero()
                        quest:AddLineToConversation(iVar11, "TEXT_QST_B12_BANDIT_FOLLOWER_ON_HIT_10", me, pCVar8, false)
                        __native_entity_state:SetStateBool("HitWarning", true)
                    elseif not quest:GetStateBool("HeroAttackedBandit") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00df87fe end
                        r3 = quest:GetNearestWithScriptName(me, "TC_BanditFighter")
                        bVar4 = quest:IsDistanceBetweenThingsUnder(me, r3, 15.0)
                        if bVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                resources:ReleaseResource(xStack_20)
                                return
                            end
                            quest:SetStateBool("HeroAttackedBandit", true)
                        end
                        quest:EntityStopFollowing(me)
                        pCVar8 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCVar8)
                        pCVar8 = quest:GetHero()
                        quest:EntityUnsetThingAsAllyOfThing(me, pCVar8)
                        pCVar9 = quest:GetHero()
                        quest:EntityUnsetThingAsAllyOfThing(pCVar9, me)
                        __native_entity_state:SetStateBool("SetAgainstHero", true)
                    end
                end
                if (not __native_entity_state:GetStateBool("SetAgainstHero")) and (quest:GetStateBool("HeroAttackedBandit")) then
                    fVar12 = 15.0
                    pCVar8 = quest:GetHero()
                    bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar12)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00df87fe end
                        iVar11 = quest:AddNewConversation(me, false, false)
                        pCVar8 = quest:GetHero()
                        quest:AddPersonToConversation(iVar11, pCVar8)
                        pCVar8 = quest:GetHero()
                        quest:AddLineToConversation(iVar11, "TEXT_QST_B12_BANDIT_FOLLOWER_SEEKING_REVENGE_10", me, pCVar8, false)
                        quest:EntityStopFollowing(me)
                        pCVar8 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, pCVar8)
                        pCVar8 = quest:GetHero()
                        quest:EntityUnsetThingAsAllyOfThing(me, pCVar8)
                        pCVar9 = quest:GetHero()
                        quest:EntityUnsetThingAsAllyOfThing(pCVar9, me)
                        __native_entity_state:SetStateBool("SetAgainstHero", true)
                    end
                end
                cVar5 = quest:GetStateBool("MissionSucceeded")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar7 = not alive
            if not bVar7 then
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

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    local cVar1 = me:MsgIsKilledBy("")
    if cVar1 then
        quest:SetMasterGameState("TCEKeepBanditFollowerAlive", false)
    end
end

