-- Readable native conversion: GTDI_Maze. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local initialAngle

-- GTDI_Maze.Main (retail 0x00e27e90)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult3, predicateResult, this_00, movie, movie2, resource2, resource3, actorMap
    local actorMap2
    if not quest:NewScriptFrame(me) then return end
    quest:SetThingHasInformation(me, false, true, false)
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntitySetAsKillable(me, false, true)
        quest:EntitySetAsDamageable(me, false)
        quest:SetIsPushableByHero(me, false)
        quest:KickOffQuestStartScreen(quest:GetActiveQuestName(), false, false)
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
        if not quest:IsActiveThreadTerminating() then
            repeat
                if me:MsgIsHitByHero() then
                    goto LAB_00e280a7
                else
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e280a7 end
                    end
                    predicateResult3 = false
                end
                goto FLOW_past_lab_00e280a7
                ::LAB_00e280a7::
                predicateResult3 = true
                ::FLOW_past_lab_00e280a7::
                if predicateResult3 then
                    if quest:IsActiveThreadTerminating() then break end
                    quest:EntitySetThingAsAllyOfThing(me, hero)
                    quest:EntitySetThingAsAllyOfThing(hero, me)
                    resource2 = resources:NewResource()
                    resources:PrepareResource(resource2)
                    if not resources:TryAcquire(resource2, hero, 4) then goto LAB_00e281e0 end
                    goto LAB_00e28214
                end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then break end
                    resource3 = resources:NewResource()
                    resources:PrepareResource(resource3)
                    if not resources:TryAcquire(resource3, hero, 4) then goto LAB_00e28460 end
                    goto LAB_00e28494
                end
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            until false
        end
    end
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00e281e0::
    while true do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            resources:ReleaseResource(resource)
            return
        end
        if resources:TryAcquire(resource2, hero, 4) then break end
    end
    ::LAB_00e28214::
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource2)
        resources:ReleaseResource(resource)
        return
    end
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource2)
        resources:ReleaseResource(resource)
        return
    end
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "MAZE", resource)
    resources:SetActor(actorMap, "HERO", resource2)
    movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_MAZE_TROPHY_INFO_SETUP", actorMap, false, false)
    resources:RunMacro("CS_MAZE_TROPHY_INFO_HITME", actorMap, false, true)
    resources:RunMacro("CS_MAZE_TROPHY_INFO", actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource2)
    goto LAB_00e28674
    ::LAB_00e28c3f::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00e28c85::
    resources:ReleaseResource(this_00)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00e28460::
    while true do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource3)
            resources:ReleaseResource(resource)
            return
        end
        if resources:TryAcquire(resource3, hero, 4) then break end
    end
    ::LAB_00e28494::
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource3)
        resources:ReleaseResource(resource)
        return
    end
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource3)
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource3)
        resources:ReleaseResource(resource)
        return
    end
    actorMap2 = resources:NewActorMap()
    resources:SetActor(actorMap2, "MAZE", resource)
    resources:SetActor(actorMap2, "HERO", resource3)
    movie2 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_MAZE_TROPHY_INFO_SETUP", actorMap2, false, false)
    resources:RunMacro("CS_MAZE_TROPHY_INFO", actorMap2, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie2)
    resources:DestroyActorMap(actorMap2)
    resources:ReleaseResource(resource3)
    ::LAB_00e28674::
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_FIND_TROPHY_DEALER", "V_TrophyDealer", false)
    quest:SetTeleporterAsActive(quest:GetThingWithScriptName("WitchwoodTeleporter"), true)
    quest:ClearThingHasInformation(me)
    quest:MiniMapRemoveMarker(me)
    quest:EntitySetFacingAngle(me, initialAngle, false)
    quest:AddLogbookStoryEntry(130)
    quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
    quest:SetStateBool("PieceOver", true)
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    repeat
        if me:MsgIsHitByHero() then
            goto LAB_00e28814
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e28814 end
            end
            predicateResult = false
        end
        goto FLOW_past_lab_00e28814
        ::LAB_00e28814::
        predicateResult = true
        ::FLOW_past_lab_00e28814::
        if predicateResult then
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetThingAsAllyOfThing(me, hero)
                quest:EntitySetThingAsAllyOfThing(hero, me)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                end
                if not quest:IsActiveThreadTerminating() then
                    local movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                    if fret_0 <= 0.0 then
                        goto LAB_00e28a13
                    end
                    goto FLOW_past_lab_00e28a13
                    ::LAB_00e28a13::
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_077_MAZE_REPEAT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e28c3f end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e28c3f end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00e28ac9
                    ::FLOW_past_lab_00e28a13::
                    if not me:Speak(hero, "TEXT_QST_077_MAZE_ON_HIT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e28c3f end
                    if not quest:IsActiveThreadTerminating() then goto LAB_00e28a13 end
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie4
                    goto LAB_00e28c85
                end
            end
            break
        end
        ::LAB_00e28ac9::
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then break end
            local movie3 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            local fret_01 = quest:GetHealth(resources:ScriptThing(resource))
            if 0.0 < fret_01 then
                me:Speak(hero, "TEXT_QST_077_MAZE_REPEAT", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00e28c81 end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00e28c81 end
                goto FLOW_past_lab_00e28c81
                ::LAB_00e28c81::
                this_00 = movie3
                goto LAB_00e28c85
                ::FLOW_past_lab_00e28c81::
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
        end
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    until false
    resources:ReleaseResource(resource)
end

-- GTDI_Maze.Init (retail 0x00e27c60)
function Init(quest, me)
    initialAngle = me:GetAngleXY()
end

-- GTDI_Maze.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- GTDI_Maze.OnPredicateFail (retail 0x00e27c80)
function OnPredicateFail(quest, me)
end

