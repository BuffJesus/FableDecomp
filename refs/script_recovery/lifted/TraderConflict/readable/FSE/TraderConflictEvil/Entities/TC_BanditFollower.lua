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
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, predicateResult8, conversationId, conversationId2, movie, resource
    if not quest:NewScriptFrame(me) then return end
    resource = resources:NewResource()
    quest:EntityAttachToScript(me, quest:GetActiveQuestName())
    quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    quest:EntitySetThingAsAllyOfThing(me, hero)
    quest:EntitySetThingAsAllyOfThing(hero, me)
    quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then goto LAB_00df87fe end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntityFollowThing(me, hero, 1.0, true)
        quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_BANDIT", 1.0)
        while not quest:GetStateBool("MissionSucceeded") do
            if not quest:NewScriptFrame(me) then goto LAB_00df87fe end
            if not state:GetBool("SetAgainstHero") then
                if not me:IsTalkedToByHero() then
                    predicateResult = false
                    goto FLOW_after_lab_00df82d0
                end
                predicateResult = true
            else
                predicateResult = false
            end
            ::FLOW_after_lab_00df82d0::
            if predicateResult then
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00df87fe end
                end
                movie = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_B12_OPENING_BANDIT_ON_SPEAK_TO", 2, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
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
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_B12_BANDIT_FOLLOWER_ON_HIT_10", me, hero, false)
                    state:SetBool("HitWarning", true)
                elseif not quest:GetStateBool("HeroAttackedBandit") then
                    if quest:IsDistanceBetweenThingsUnder(me, quest:GetNearestWithScriptName(me, "TC_BanditFighter"), 15.0) then
                        quest:SetStateBool("HeroAttackedBandit", true)
                    end
                    quest:EntityStopFollowing(me)
                    quest:GiveThingBestEnemyTarget(me, hero)
                    quest:EntityUnsetThingAsAllyOfThing(me, hero)
                    quest:EntityUnsetThingAsAllyOfThing(hero, me)
                    state:SetBool("SetAgainstHero", true)
                end
            end
            if not state:GetBool("SetAgainstHero") and quest:GetStateBool("HeroAttackedBandit") then
                if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00df87fe end
                    conversationId2 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId2, hero)
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_B12_BANDIT_FOLLOWER_SEEKING_REVENGE_10", me, hero, false)
                    quest:EntityStopFollowing(me)
                    quest:GiveThingBestEnemyTarget(me, hero)
                    quest:EntityUnsetThingAsAllyOfThing(me, hero)
                    quest:EntityUnsetThingAsAllyOfThing(hero, me)
                    state:SetBool("SetAgainstHero", true)
                end
            end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:RemoveThing(me, false, true)
        end
    end
    ::LAB_00df87fe::
    resources:ReleaseResource(resource)
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

