-- Readable native conversion: MazeAtTavern. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local wavedOver, hitCount, talkCounter

-- MazeAtTavern.Main (retail 0x00e25f70)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, scratchValue, scratchValue4, scratchValue5, newResource
    if not quest:NewScriptFrame(me) then return end
    newResource = resources:NewResource()
    resources:PrepareResource(newResource)
    while not resources:TryAcquire(newResource, me, 3) do
        if not quest:NewScriptFrame(me) then goto LAB_00e26761 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e26761 end
    quest:EntitySetAsOpinionSource(me, "OPINION_SOURCE_MAZE_BS_PUB")
    quest:GetThingWithScriptName("M_MazeExit"):GetPos()
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetIsPushableByHero(me, false)
    scratchValue = quest:ReadGlobalGameData(3176)
    scratchValue5 = 0
    if quest:IsActiveThreadTerminating() then goto LAB_00e26761 end
    repeat
        if not quest:GetStateBool("GuardianSpokeToHero") and not wavedOver then
            if quest:IsDistanceBetweenThingsUnder(hero, me, scratchValue) then
                if quest:IsActiveThreadTerminating() then break end
                me:PlayAnimation("ST_HELLO", false, false, false, true, true, false, false)
                local conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, hero)
                quest:AddLineToConversation(conversationId, "TEXT_QST_027_MAZE_CALL_HERO_OVER_10", me, hero, false)
                wavedOver = true
                scratchValue5 = newResource
            end
        end
        if not quest:GetStateBool("GuardianSpokeToHero") then
            if quest:IsActiveThreadTerminating() then break end
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
        end
        scratchValue4 = scratchValue5 | 1
        newResource = scratchValue4
        if me:MsgIsHitByHero() then
            goto LAB_00e262d9
        else
            scratchValue4 = scratchValue5 | 3
            newResource = scratchValue4
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue4 = scratchValue5 | 7
                newResource = scratchValue4
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e262d9 end
            end
            predicateResult = false
        end
        goto FLOW_past_lab_00e262d9
        ::LAB_00e262d9::
        predicateResult = true
        ::FLOW_past_lab_00e262d9::
        if scratchValue4 & 4 ~= 0 then
            scratchValue4 = scratchValue4 & 0xfffffffb
            newResource = scratchValue4
        end
        if scratchValue4 & 2 ~= 0 then
            scratchValue4 = scratchValue4 & 0xfffffffd
            newResource = scratchValue4
        end
        if scratchValue4 & 1 ~= 0 then
            -- TODO(native): xStack_8c = uVar10 & 0xfffffffe;
        end
        if predicateResult then
            if quest:IsActiveThreadTerminating() then break end
            resources:PrepareResource(newResource)
            while not resources:TryAcquire(newResource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00e26761 end
            end
            if quest:IsActiveThreadTerminating() then break end
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local fret_0 = quest:GetHealth(resources:ScriptThing(newResource))
            if 0.0 < fret_0 then
                me:Speak(hero, "TEXT_QST_027_MAZE_ON_HIT_10", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e26754 end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e26754 end
                goto FLOW_past_lab_00e26754
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
            resources:PrepareResource(newResource)
            while not resources:TryAcquire(newResource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00e26761 end
            end
            if quest:IsActiveThreadTerminating() then break end
            local resource = resources:NewResource()
            resources:TryAcquire(resource, hero, 4)
            local actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "ME", newResource)
            resources:SetActor(actorMap, "HERO", resource)
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_GUARDIANSISTER_BOWERSTONE", actorMap, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            quest:SetStateBool("GuardianSpokeToHero", true)
            quest:ClearThingHasInformation(me)
            quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            quest:RemoveThing(me, false, true)
        end
        quest:NewScriptFrame(me)
        scratchValue5 = newResource
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(newResource)
            do return end
        end
    until false
    ::LAB_00e26761::
    resources:ReleaseResource(newResource)
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
    if quest:GetStateBool("GuardianSpokeToHero") then
        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    end
end

