-- Readable native conversion: MazeAtTavern. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GSI_MazeCallsOverDistance = 3176,  -- 10
}

-- per-entity fields (native class members; one Lua state per entity instance)
local wavedOver, hitCount, talkCounter

-- MazeAtTavern.Main (retail 0x00e25f70)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, cleanupFlags, scratchValue4, scratchValue5
    cleanupFlags = 0
    if not quest:NewScriptFrame(me) then return end
    local resource3 = resources:NewResource()
    resources:PrepareResource(resource3)
    while not resources:TryAcquire(resource3, me, 3) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource3); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); return end
    quest:EntitySetAsOpinionSource(me, "OPINION_SOURCE_MAZE_BS_PUB")
    quest:GetThingWithScriptName("M_MazeExit"):GetPos()
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetIsPushableByHero(me, false)
    local scratchValue = quest:ReadGlobalGameData(SCRIPT_DEF.GSI_MazeCallsOverDistance)
    scratchValue5 = 0
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); return end
    repeat
        if not quest:GetStateBool("GuardianSpokeToHero") and not wavedOver then
            local hero = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(hero, me, scratchValue) then
                if quest:IsActiveThreadTerminating() then break end
                resources:PlayAnimationWithNativeArgument5(resource3, "ST_HELLO", false, false, false, true, false, false)
                local conversationId = quest:AddNewConversation(me, false, false)
                local hero2 = quest:GetHero()
                quest:AddPersonToConversation(conversationId, hero2)
                local hero3 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_027_MAZE_CALL_HERO_OVER_10", me, hero3, false)
                wavedOver = true
                scratchValue5 = cleanupFlags
            end
        end
        if not quest:GetStateBool("GuardianSpokeToHero") then
            if quest:IsActiveThreadTerminating() then break end
            local hero4 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, hero4, false)
        end
        scratchValue4 = scratchValue5 | 1
        cleanupFlags = scratchValue4
        if me:MsgIsHitByHero() then
            goto LAB_00e262d9
        else
            scratchValue4 = scratchValue5 | 3
            cleanupFlags = scratchValue4
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue4 = scratchValue5 | 7
                cleanupFlags = scratchValue4
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e262d9 end
            end
            predicateResult = false
        end
        goto FLOW_past_lab_00e262d9
        ::LAB_00e262d9::
        predicateResult = true
        ::FLOW_past_lab_00e262d9::
        if (scratchValue4 & 4) ~= 0 then
            scratchValue4 = scratchValue4 & 0xfffffffb
            cleanupFlags = scratchValue4
        end
        if (scratchValue4 & 2) ~= 0 then
            scratchValue4 = scratchValue4 & 0xfffffffd
            cleanupFlags = scratchValue4
        end
        if (scratchValue4 & 1) ~= 0 then
            cleanupFlags = scratchValue4 & 0xfffffffe
        end
        if predicateResult then
            if quest:IsActiveThreadTerminating() then break end
            resources:PrepareResource(resource3)
            while not resources:TryAcquire(resource3, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource3); return end
            end
            if quest:IsActiveThreadTerminating() then break end
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local scratchValue6 = resources:ScriptThing(resource3)
            local fret_0 = quest:GetHealth(scratchValue6)
            if 0.0 < fret_0 then
                local p1 = "TEXT_QST_027_MAZE_ON_HIT_10"
                local hero5 = quest:GetHero()
                if not me:Speak(hero5, p1, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                else
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e26754 end
                    end
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e26754 end
                    goto FLOW_past_lab_00e26754
                end
                ::LAB_00e26754::
                resources:DestroyMovie(movie2)
                break
                ::FLOW_past_lab_00e26754::
            end
            hitCount = hitCount + 1
            me:SetFriendsWithEverythingFlag(true)
            quest:ClearThingBestEnemyTarget(me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then break end
            resources:PrepareResource(resource3)
            while not resources:TryAcquire(resource3, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource3); return end
            end
            if quest:IsActiveThreadTerminating() then break end
            local resource2 = resources:NewResource()
            local resource = resource2
            local hero6 = quest:GetHero()
            resources:TryAcquire(resource, hero6, 4)
            local actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "ME", resource3)
            resources:SetActor(actorMap, "HERO", resource2)
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_GUARDIANSISTER_BOWERSTONE", actorMap, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource2)
            quest:SetStateBool("GuardianSpokeToHero", true)
            quest:ClearThingHasInformation(me)
            local getActiveQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(getActiveQuestName, false, false, false)
            local getActiveQuestName2 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(getActiveQuestName2, 0)
            quest:RemoveThing(me, false, true)
        end
        quest:NewScriptFrame(me)
        scratchValue5 = cleanupFlags
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource3)
            do return end
        end
    until false
    resources:ReleaseResource(resource3)
end

-- MazeAtTavern.Init (retail 0x00e25d20)
function Init(quest, me)
    talkCounter = 0
    wavedOver = false
    hitCount = 0
end

-- MazeAtTavern.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- MazeAtTavern.OnPredicateFail (retail 0x00e25d30)
function OnPredicateFail(quest, me)
    if not quest:GetStateBool("GuardianSpokeToHero") then return end
    local pQuestName = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pQuestName, 0)
end

