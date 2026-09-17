-- Readable native conversion: TC_BanditFollower. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- TC_BanditFollower.Main (retail 0x00df80b0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult5, predicateResult8, conversationId, conversationId2, scratchValue5
    local scratchValue6
    if not quest:NewScriptFrame(me) then return end
    scratchValue6 = resources:NewResource()
    quest:EntityAttachToScript(me, quest:GetActiveQuestName())
    quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    quest:EntitySetThingAsAllyOfThing(me, quest:GetHero())
    quest:EntitySetThingAsAllyOfThing(quest:GetHero(), me)
    quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then goto LAB_00df87fe end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntityFollowThing(me, quest:GetHero(), 1.0, true)
        quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_BANDIT", 1.0)
        while not quest:GetStateBool("MissionSucceeded") do
            if not quest:NewScriptFrame(me) then goto LAB_00df87fe end
            if not state:GetBool("SetAgainstHero") then
                if not me:IsTalkedToByHero() then
                    predicateResult5 = false
                    goto FLOW_after_lab_00df82d0
                end
                predicateResult5 = true
            else
                predicateResult5 = false
            end
            ::FLOW_after_lab_00df82d0::
            if predicateResult5 then
                while not resources:TryAcquire(scratchValue6, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00df87fe end
                end
                scratchValue5 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue6)) then
                    me:Speak(quest:GetHero(), "TEXT_QST_B12_OPENING_BANDIT_ON_SPEAK_TO", 2, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue5)
                            resources:ReleaseResource(scratchValue6)
                            return
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(scratchValue5)
                        resources:DestroyMovie(scratchValue6)
                        return
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue5)
            end
            if not state:GetBool("SetAgainstHero") then
                if not me:MsgIsHitByHero() then
                    if not (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(me)) then
                        predicateResult8 = false
                        goto FLOW_after_lab_00df851c
                    end
                end
                predicateResult8 = true
            else
                predicateResult8 = false
            end
            ::FLOW_after_lab_00df851c::
            if predicateResult8 then
                if quest:IsActiveThreadTerminating() then goto LAB_00df87fe end
                if not state:GetBool("HitWarning") then
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, quest:GetHero())
                    quest:AddLineToConversation(conversationId, "TEXT_QST_B12_BANDIT_FOLLOWER_ON_HIT_10", me, quest:GetHero(), false)
                    state:SetBool("HitWarning", true)
                elseif not quest:GetStateBool("HeroAttackedBandit") then
                    if quest:IsDistanceBetweenThingsUnder(me, quest:GetNearestWithScriptName(me, "TC_BanditFighter"), 15.0) then
                        quest:SetStateBool("HeroAttackedBandit", true)
                    end
                    quest:EntityStopFollowing(me)
                    quest:GiveThingBestEnemyTarget(me, quest:GetHero())
                    quest:EntityUnsetThingAsAllyOfThing(me, quest:GetHero())
                    quest:EntityUnsetThingAsAllyOfThing(quest:GetHero(), me)
                    state:SetBool("SetAgainstHero", true)
                end
            end
            if not state:GetBool("SetAgainstHero") and quest:GetStateBool("HeroAttackedBandit") then
                if quest:IsDistanceBetweenThingsUnder(me, quest:GetHero(), 15.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00df87fe end
                    conversationId2 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId2, quest:GetHero())
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_B12_BANDIT_FOLLOWER_SEEKING_REVENGE_10", me, quest:GetHero(), false)
                    quest:EntityStopFollowing(me)
                    quest:GiveThingBestEnemyTarget(me, quest:GetHero())
                    quest:EntityUnsetThingAsAllyOfThing(me, quest:GetHero())
                    quest:EntityUnsetThingAsAllyOfThing(quest:GetHero(), me)
                    state:SetBool("SetAgainstHero", true)
                end
            end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:RemoveThing(me, false, true)
        end
    end
    ::LAB_00df87fe::
    resources:ReleaseResource(scratchValue6)
end

-- TC_BanditFollower.Init (retail 0x00df8040)
function Init(quest, me)
    state:SetBool("HitWarning", false)
    state:SetBool("SetAgainstHero", false)
end

-- TC_BanditFollower.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TC_BanditFollower.OnPredicateFail (retail 0x00df8050)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") then
        quest:SetMasterGameState("TCEKeepBanditFollowerAlive", false)
    end
end

