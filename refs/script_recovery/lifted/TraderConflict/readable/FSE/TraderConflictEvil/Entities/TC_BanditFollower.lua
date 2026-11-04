-- Readable native conversion: TC_BanditFollower. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local setAgainstHero, hitWarning

-- TC_BanditFollower.Main (retail 0x00df80b0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, healthBar
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    local pQuestName = quest:GetActiveQuestName()
    quest:EntityAttachToScript(me, pQuestName)
    quest:EntitySetInFaction(me, "FACTION_BANDITS_FRIENDLY")
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)
    local hero13 = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(hero13, me)
    quest:EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(me, false)
    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local hero2 = quest:GetHero()
    quest:EntityFollowThing(me, hero2, 1.0, true)
    healthBar = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_BANDIT", 1.0)
    while not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        if not (setAgainstHero or (not me:IsTalkedToByHero())) then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local scratchValue = resources:ScriptThing(resource)
            local fret_0 = quest:GetHealth(scratchValue)
            if 0.0 < fret_0 then
                local p1 = "TEXT_QST_B12_OPENING_BANDIT_ON_SPEAK_TO"
                local hero3 = quest:GetHero()
                if not me:Speak(hero3, p1, 2, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    return
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        resources:ReleaseResource(resource)
                        do return end
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
        if not (setAgainstHero) then
            if not me:MsgIsHitByHero() then
                if not (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL))) then
                    goto LAB_00df851c
                end
            end
            predicateResult = true
            goto FLOW_past_lab_00df851c
        end
        ::LAB_00df851c::
        predicateResult = false
        ::FLOW_past_lab_00df851c::
        if predicateResult then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if not hitWarning then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local conversationId = quest:AddNewConversation(me, false, false)
                local hero4 = quest:GetHero()
                quest:AddPersonToConversation(conversationId, hero4)
                local hero5 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_B12_BANDIT_FOLLOWER_ON_HIT_10", me, hero5, false)
                hitWarning = true
            elseif not quest:GetStateBool("HeroAttackedBandit") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local banditFighter = quest:GetNearestWithScriptName(me, "TC_BanditFighter")
                if quest:IsDistanceBetweenThingsUnder(me, banditFighter, 15.0) then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    quest:SetStateBool("HeroAttackedBandit", true)
                end
                quest:EntityStopFollowing(me)
                local hero6 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(me, hero6)
                local hero7 = quest:GetHero()
                quest:EntityUnsetThingAsAllyOfThing(me, hero7)
                local hero14 = quest:GetHero()
                quest:EntityUnsetThingAsAllyOfThing(hero14, me)
                setAgainstHero = true
            end
        end
        if not setAgainstHero and quest:GetStateBool("HeroAttackedBandit") then
            local hero8 = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(me, hero8, 15.0) then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local conversationId2 = quest:AddNewConversation(me, false, false)
                local hero9 = quest:GetHero()
                quest:AddPersonToConversation(conversationId2, hero9)
                local hero10 = quest:GetHero()
                quest:AddLineToConversation(conversationId2, "TEXT_QST_B12_BANDIT_FOLLOWER_SEEKING_REVENGE_10", me, hero10, false)
                quest:EntityStopFollowing(me)
                local hero11 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(me, hero11)
                local hero12 = quest:GetHero()
                quest:EntityUnsetThingAsAllyOfThing(me, hero12)
                local hero15 = quest:GetHero()
                quest:EntityUnsetThingAsAllyOfThing(hero15, me)
                setAgainstHero = true
            end
        end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:RemoveThing(me, false, true)
    end
    resources:ReleaseResource(resource)
end

-- TC_BanditFollower.Init (retail 0x00df8040)
function Init(quest, me)
    hitWarning = false
    setAgainstHero = false
end

-- TC_BanditFollower.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TC_BanditFollower.OnPredicateFail (retail 0x00df8050)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") then
        quest:SetMasterGameState("TCEKeepBanditFollowerAlive", false)
    end
end

